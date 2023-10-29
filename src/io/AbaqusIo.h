#include "Io.h"



// concrete class for IO
class AbaqusIo : public Io
{
public:
    virtual void write(const char* data) override;
    virtual void read(char* data) override;
};