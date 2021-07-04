#include <QtWidgets/qapplication.h>
#include "circle_loading_widget.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    CircleLoadingWidget w;
    w.show();
    w.start();
    return app.exec();
}
