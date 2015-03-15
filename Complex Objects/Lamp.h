#include <vector>
#include "../Simple Objects/Cylinder.h"
#include "../Simple Objects/Torus.h"
using namespace std;

class Lamp{
public:
    ~Lamp();
    void build();
    void render()const;
};