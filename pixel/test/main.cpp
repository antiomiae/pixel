#define CATCH_CONFIG_RUNNER
#include "setup.h"
#include <pixel/pixel.h>
#include <boost/filesystem.hpp>


using namespace std;

int main(int argc, char** argv)
{


    if (argc >= 2) {
        cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
        argc -= 1;
        argv = &argv[1];
    }

    cout << "Working directory: " << boost::filesystem::current_path().string() << endl;

    pixeltest::setup();

    int result = Catch::Session().run( argc, argv );

    pixeltest::teardown();

    return result;
}
