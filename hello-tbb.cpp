#include <iostream>
#include <tbb/parallel_for.h>

using namespace tbb;
using namespace std;

class hello {
public:
    void operator()(int x) const {
        cout << "Hello world!  TBB version is " << TBB_VERSION_MAJOR << 
            "." << TBB_VERSION_MINOR << ", interface " << 
            (float)TBB_INTERFACE_VERSION/1000.0 <<"\n";
    }
};

int main() {
    parallel_for( 0, 5, 1, hello() );
    return 0;
}
