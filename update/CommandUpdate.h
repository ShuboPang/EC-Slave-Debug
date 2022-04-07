#ifndef __COMMANDUPDATE_H__
#define __COMMANDUPDATE_H__

#include "SdoCommand.h"

class CommandUpdate:
    public SdoCommand
{

public:
    CommandUpdate();
    virtual void execute(const StringVector &);
    virtual string helpString(const string &) const;

};

#endif 
