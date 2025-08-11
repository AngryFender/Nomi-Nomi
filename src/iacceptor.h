#ifndef IACCEPTOR_H
#define IACCEPTOR_H
#include <memory>
#include "iconnection.h"
class IAcceptor
{
public:
     virtual ~IAcceptor() = default;
     virtual void setHandler(std::function<void(std::shared_ptr<Iconnection>)> handler) = 0;
     virtual void open() = 0;
};
#endif //IACCEPTOR_H
