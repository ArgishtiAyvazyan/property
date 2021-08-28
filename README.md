# Property
The property in C++

In other programming languages, a field and the set and get methods associated with it are called properties. This simple library in C++ helps to property declaration and auto-generates the public or private access methods for it. The implementation is based on Property Proxy design patterns.

## Demo

```cpp

#include "property.h"

struct dummy_object
{
    template <class ... TArgs>
    using property_t = util::property <dummy_object, TArgs...>;
    
    // The property can change anyone.
    property_t<int, util::public_get_set> public_read_write;
    
    // The property can read anyone but change only inside of dummy_object class.
    property_t<int, util::public_get> public_read;
    
    // The access to this object has only inside of dummy_object class.
    property_t<int> private_property;
};

int main()
{
    dummy_object obj;
    int val = obj.public_read_write; // Ok.
    obj.public_read_write = 13;      // Ok.
    val = obj.public_read;           // Ok.
    obj.public_read = 13;            // Compile error.
    val = obj.private_property;      // Compile error.
    obj.private_property = 10;       // Compile error.
    return 0;
}

```

## Use guide:

The class property:
```cpp
template <typename TOwner, typename TValue, typename TAccessPolicy>
class property { ... };
```
* TOwner - is the type of owner, which should have all accesses. 
* TValue - is the type of property value.
* TAccessPolicy - is the access policy for the property. The param is optional default value is private_get_set. Possible variants: 
  * public_get - Declare only a public get accessor and private set.
  * public_get_set - Declare only a public get and set accessors.
  * private_get_set - Declare only a private get and set accessors.
                     
### Build:

```bash
mkdir build
cd ./build
cmake ..
make -j <job count>
```

### Build unit tests.
```bash
mkdir build
cd ./build
cmake -DBUILD_UNIT_TESTS=YES ..
make -j <job count>
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

