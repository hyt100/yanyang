#include "App.h"

int main(int argc, char *argv[])
{
    yyCoreInstance.setupWindow("example", 1024, 1024);
    yyCoreInstance.init();
    return yyCoreInstance.run(new App());
}
