#include "header.h"

void _API InDLLDestruct( void * );
_API unsigned char *InDLLConstruct( int size );

template<typename T>
struct server_object_container
{
  int real_size;
  unsigned char *memory;
public:
  server_object_container() : memory(InDLLConstruct(real_size)), real_size(sizeof(T))
  {}
  server_object_container( int _size ) : memory(InDLLConstruct(real_size)), real_size(_size)
  {}
  server_object_container(const server_object_container &obj) : memory(InDLLConstruct(real_size)), real_size(obj.real_size)
  {
    Import(obj.memory);
  }
  server_object_container( const T &obj ) : memory(InDLLConstruct(real_size)), real_size(sizeof(T))
  {
    Import(&obj);
  }
  server_object_container(const T &obj, const int size) : memory(InDLLConstruct(real_size)), real_size(size)
  {
    Import(&obj);
  }

  ~server_object_container()
  {
    InDLLDestruct(memory);
  }

  template<typename OtherT>
  server_object_container( const server_object_container<OtherT> &obj ) : memory(InDLLConstruct(real_size)), real_size(obj.Size())
  {
    Import(obj.Memory());
  }

  int Size() const
  {
    return real_size;
  }

  const unsigned char *Memory() const
  {
    return memory;
  }

  T &operator*()
  {
    return reinterpret_cast<T *>(memory);
  }
  T *operator->()
  {
    return reinterpret_cast<T *>(memory);
  }

  const T &operator*() const
  {
    return *reinterpret_cast<T *>(memory);
  }
  const T *operator->() const
  {
    return reinterpret_cast<T *>(memory);
  }
private:
  void Import( const void *mem )
  {
    memcpy(memory, mem, real_size);
  }
};