#ifndef SERVICEONER_H
#define SERVICEONER_H
#include <qwidget.h>

class ServiceOner: public QWidget
{
    Q_OBJECT
public:
    ServiceOner(const char* name);
public slots:
      void startService(void);
};

#endif // SERVICEONER_H
