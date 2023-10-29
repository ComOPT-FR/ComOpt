
#include "Io.h"


// concrete class from IO 
class ComOptIo : public Io
{
public:
    virtual void write(const char* data) override;
    virtual void read(char* data) override;
};