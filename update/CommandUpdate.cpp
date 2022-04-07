#include <libgen.h> // basename()
#include <string.h>
#include <iomanip>
#include <iostream>
#include <fstream>
using namespace std;


#include "CommandUpdate.h"
#include "MasterDevice.h"

CommandUpdate::CommandUpdate():
    SdoCommand("update", "update a slave app.")
{
}


string CommandUpdate::helpString(const string &binaryBaseName) const
{
    stringstream str;

    str << binaryBaseName << " " << getName()
        << " [OPTIONS] <FILENAME> <OFFSET> <BUFFSIZE>" << endl
        << endl
        << getBriefDescription() << endl
        << endl
        << "This command requires a single slave to be selected." << endl
        << endl
        << "The OFFSET is used for writing part of the flash." << endl
        << endl
        << "The BUFFSIZE is the size for each flash." << endl
        << endl
        << "Arguments:" << endl
        << "  FILENAME app for update" << endl
        << "  OFFSET start addr for writing" << endl
        << endl
        << "Command-specific options:" << endl
        << "  --alias    -a <alias>" << endl
        << "  --position -p <pos>    Slave selection. See the help of" << endl
        << "                         the 'slaves' command." << endl
        << endl;

    return str.str();
}

/****************************************************************************/

void CommandUpdate::execute(const StringVector &args)
{

    stringstream str_offset, str_buffsize, err;
//  ec_ioctl_slave_sdo_download_t data;
//  unsigned int valueIndex;
//  const DataType *dataType = NULL;
    SlaveList slaves;
//
    if (args.size() != 3) {
        err << "'" << getName() << "' takes 3 arguments!";
        throwInvalidUsageException(err);
    }
    ifstream in;
    in.open(args[0].c_str(), ios::in | ios::binary);
    // get length of file:
    in.seekg (0, in.end);
    int length = in.tellg();
    in.seekg (0, in.beg);
    char * fileContent = new char [length];
    in.read (fileContent,length);
    in.close();

    ec_ioctl_slave_sdo_upload_t read_can_sent_flag;
    ec_ioctl_slave_sdo_download_t data_addr;
    ec_ioctl_slave_sdo_download_t data;
    ec_ioctl_slave_sdo_download_t write_can_sent_flag;
//  ec_ioctl_slave_sdo_download_t app_crc;


    read_can_sent_flag.sdo_index = 0x5000;
    read_can_sent_flag.sdo_entry_subindex = 1;

    write_can_sent_flag.complete_access = 0;
    write_can_sent_flag.sdo_index = 0x5000;
    write_can_sent_flag.sdo_entry_subindex = 1;

    data_addr.complete_access = 0;
    data_addr.sdo_index = 0x5000;
    data_addr.sdo_entry_subindex = 2;

    data.complete_access = 0;
    data.sdo_index = 0x5001;
    data.sdo_entry_subindex = 0;

//  app_crc.complete_access = 0;
//  app_crc.sdo_index = 0x5000;
//  app_crc.sdo_entry_subindex = 4;

    int offset;
    str_offset << args[1];
    str_offset >> resetiosflags(ios::basefield)>> offset;

    if (str_offset.fail()) {
        err << "Invalid Offset '" << args[1] << "'!";
        throwInvalidUsageException(err);
    }

    int buffSize;
    str_buffsize << args[2];
    str_buffsize >> resetiosflags(ios::basefield)>> buffSize;

    if (str_buffsize.fail()) {
        err << "Invalid BuffSize '" << args[2] << "'!";
        throwInvalidUsageException(err);
    }


    MasterDevice m(getSingleMasterIndex());
    m.open(MasterDevice::ReadWrite);
    slaves = selectedSlaves(m);
    if (slaves.size() != 1) {
        throwSingleSlaveRequired(slaves.size());
    }

    uint16_t slave_postion = slaves.front().position;

    read_can_sent_flag.slave_position = slave_postion;
    write_can_sent_flag.slave_position = slave_postion;
    data_addr.slave_position = slave_postion;
    data.slave_position = slave_postion;
//  app_crc.slave_position = slave_postion;

    read_can_sent_flag.target_size = 4;
    write_can_sent_flag.data_size = 4;
    data_addr.data_size = 4;
    data.data_size = buffSize;
//  app_crc.data_size = 4;

    read_can_sent_flag.target = new uint8_t[read_can_sent_flag.target_size + 1];
    write_can_sent_flag.data = new uint8_t[write_can_sent_flag.data_size + 1];
    data_addr.data = new uint8_t[data_addr.data_size + 1];
    data.data = new uint8_t[data.data_size + 1];
//  app_crc.data = new uint8_t[app_crc.data_size + 1];

    // start update
    while (1) {
        cout<<"Wait to enter update start:"<<length<<endl;
        write_can_sent_flag.data[0] = 3;
        write_can_sent_flag.data[1] = 0;
        m.sdoDownload(&write_can_sent_flag);
        m.sdoUpload(&read_can_sent_flag);
        if (read_can_sent_flag.target[0] == 3) {
            while (1) {
                write_can_sent_flag.data[0] = 0x55;
                write_can_sent_flag.data[1] = 0;
                m.sdoDownload(&write_can_sent_flag);
                int sec_count = length / (64 * 1024) + 1;
                cout<<"use blcok count:"<<sec_count<<endl;
                for (int i = 0; i < sec_count;++i) {
                    write_can_sent_flag.data[0] = 0x56;
                    write_can_sent_flag.data[1] = 0;
                    write_can_sent_flag.data[2] = i;
                    write_can_sent_flag.data[3] = 1;
                    m.sdoDownload(&write_can_sent_flag);
                }
                
                m.sdoUpload(&read_can_sent_flag);
                if (read_can_sent_flag.target[0] == 0) {
                    break;
                }
            }
            cout << "Entered update process!"<<endl;
            break;
        }
    }
    
    int sent_count = (length - offset) / data.data_size;
    int last_sent = (length - offset) % data.data_size;
    char *fp = fileContent + offset;
    cout<<"sent info:"<<sent_count<<" mod:"<<last_sent<<endl;
    for (int i = 0; i < sent_count; ++i) {
        cout<<setw(5)<<i;
        cout<<"/"<<sent_count;
        putchar('\r');
        m.sdoUpload(&read_can_sent_flag);
        if (read_can_sent_flag.target[0] == 0) {
            data_addr.data[0] = i & 0xFF;
            data_addr.data[1] = (i >> 8) & 0xFF;
            memcpy(data.data, fp,data.data_size);
            write_can_sent_flag.data[0] = 1;
            write_can_sent_flag.data[1] = 0;
            m.sdoDownload(&data_addr);
            m.sdoDownload(&data);
            m.sdoDownload(&write_can_sent_flag);
            fp += data.data_size;
            
        }
        else
        {
//          --i;
        }
    }

    m.sdoUpload(&read_can_sent_flag);
    if (read_can_sent_flag.target[0] == 0) {
        data_addr.data[0] = sent_count & 0xFF;
        data_addr.data[1] = (sent_count >> 8) & 0xFF;
        memcpy(data.data, fp,last_sent);
        write_can_sent_flag.data[0] = 1;
        write_can_sent_flag.data[1] = 0;
        m.sdoDownload(&data_addr);
        m.sdoDownload(&data);
        m.sdoDownload(&write_can_sent_flag);

        
    }
    else
    {
//      --i;
    }

    write_can_sent_flag.data[0] = 2;
    write_can_sent_flag.data[1] = 0;
    m.sdoDownload(&write_can_sent_flag);
    
    delete[] read_can_sent_flag.target;
    delete[] data_addr.data;
    delete[] data.data;
    delete[] write_can_sent_flag.data;
//  data.complete_access = args.size() == 2;
//  valueIndex = data.complete_access ? 1 : 2;
//
//  strIndex << args[0];
//  strIndex
//      >> resetiosflags(ios::basefield) // guess base from prefix
//      >> data.sdo_index;
//  if (strIndex.fail()) {
//      err << "Invalid SDO index '" << args[0] << "'!";
//      throwInvalidUsageException(err);
//  }
//
//  if (data.complete_access) {
//      data.sdo_entry_subindex = 0;
//  } else {
//      stringstream strSubIndex;
//      unsigned int number;
//
//      strSubIndex << args[1];
//      strSubIndex
//          >> resetiosflags(ios::basefield) // guess base from prefix
//          >> number;
//      if (strSubIndex.fail() || number > 0xff) {
//          err << "Invalid SDO subindex '" << args[1] << "'!";
//          throwInvalidUsageException(err);
//      }
//      data.sdo_entry_subindex = number;
//  }
//
//  MasterDevice m(getSingleMasterIndex());
//  m.open(MasterDevice::ReadWrite);
//  slaves = selectedSlaves(m);
//  if (slaves.size() != 1) {
//      throwSingleSlaveRequired(slaves.size());
//  }
//  data.slave_position = slaves.front().position;
//
//  if (!getDataType().empty()) { // data type specified
//      if (!(dataType = findDataType(getDataType()))) {
//          err << "Invalid data type '" << getDataType() << "'!";
//          throwInvalidUsageException(err);
//      }
//  } else { // no data type specified: fetch from dictionary
//      ec_ioctl_slave_sdo_entry_t entry;
//
//      try {
//          m.getSdoEntry(&entry, data.slave_position,
//                  data.sdo_index, data.sdo_entry_subindex);
//      } catch (MasterDeviceException &e) {
//          err << "Failed to determine SDO entry data type. "
//              << "Please specify --type.";
//          throwCommandException(err);
//      }
//      if (!(dataType = findDataType(entry.data_type))) {
//          err << "PDO entry has unknown data type 0x"
//              << hex << setfill('0') << setw(4) << entry.data_type << "!"
//              << " Please specify --type.";
//          throwCommandException(err);
//      }
//  }
//
//  if (args[valueIndex] == "-") {
//      ostringstream tmp;
//
//      tmp << cin.rdbuf();
//      string const &contents = tmp.str();
//
//      if (!contents.size()) {
//          err << "Invalid data size " << contents.size() << "! "
//              << "Must be non-zero.";
//          throwCommandException(err);
//      }
//      data.data_size = contents.size();
//      data.data = new uint8_t[data.data_size + 1];
//
//      try {
//          data.data_size = interpretAsType(
//                  dataType, contents, data.data, data.data_size);
//      } catch (SizeException &e) {
//          delete [] data.data;
//          throwCommandException(e.what());
//      } catch (ios::failure &e) {
//          delete [] data.data;
//          err << "Invalid value argument '" << args[2]
//              << "' for type '" << dataType->name << "'!";
//          throwInvalidUsageException(err);
//      }
//
//  } else {
//      if (dataType->byteSize) {
//          data.data_size = dataType->byteSize;
//      } else {
//          data.data_size = DefaultBufferSize;
//      }
//
//      data.data = new uint8_t[data.data_size + 1];
//
//      try {
//          data.data_size = interpretAsType(
//                  dataType, args[valueIndex], data.data, data.data_size);
//      } catch (SizeException &e) {
//          delete [] data.data;
//          throwCommandException(e.what());
//      } catch (ios::failure &e) {
//          delete [] data.data;
//          err << "Invalid value argument '" << args[2]
//              << "' for type '" << dataType->name << "'!";
//          throwInvalidUsageException(err);
//      }
//  }
//
//  try {
//      data.data_size = interpretAsType(
//              dataType, args[valueIndex], data.data, data.data_size);
//  } catch (SizeException &e) {
//      delete [] data.data;
//      throwCommandException(e.what());
//  } catch (ios::failure &e) {
//      delete [] data.data;
//      err << "Invalid value argument '" << args[2]
//          << "' for type '" << dataType->name << "'!";
//      throwInvalidUsageException(err);
//  }
//
//  try {
//      m.sdoDownload(&data);
//  } catch (MasterDeviceSdoAbortException &e) {
//      delete [] data.data;
//      err << "SDO transfer aborted with code 0x"
//          << setfill('0') << hex << setw(8) << e.abortCode
//          << ": " << abortText(e.abortCode);
//      throwCommandException(err);
//  } catch(MasterDeviceException &e) {
//      delete [] data.data;
//      throw e;
//  }
//
//  delete [] data.data;
}

