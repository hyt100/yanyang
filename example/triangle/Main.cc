#include "App.h"

int main(int argc, char *argv[])
{
    yyCoreInstance.setupWindow("example", 512, 512);
    yyCoreInstance.init();
    return yyCoreInstance.run(new App());
}
