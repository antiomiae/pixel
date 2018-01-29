//
//

#include "setup.h"

namespace
{

TEST(SetupTest, AppIsInitialized)
{

    ASSERT_NO_THROW(
            pixeltest::app()
    );

}

};

