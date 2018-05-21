#include "setup.h"
#include <boost/filesystem.hpp>

using namespace std;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    if (argc >= 2) {
        cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
    }

    cout << "Working directory: " << boost::filesystem::current_path().string() << endl;

    pixeltest::setup();

    pixel::app().update_render_context();

    int ret = RUN_ALL_TESTS();

    pixeltest::teardown();

    return ret;
}
