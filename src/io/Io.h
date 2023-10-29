




// abstract class Io
class Io {
public:
    virtual ~Io() {}
    virtual void write(const char* data) = 0;
    virtual void read(char* data) = 0;
};



