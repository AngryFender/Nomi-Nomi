#ifndef ICONNECTOR_H
#define ICONNECTOR_H

class IConnector
{
public:
    virtual ~IConnector() = default;
    virtual bool start() = 0;;
};

#endif //ICONNECTOR_H
