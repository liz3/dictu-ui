#ifndef NOTIFICATION_C_INTERFACE_H__
#define NOTIFICATION_C_INTERFACE_H__

#ifdef __OBJC__
extern "C" {
#endif
typedef struct {
  double lat, lng;
  int was_set;
} Location;
void getLocation(Location *loc);
#ifdef __OBJC__
}
#endif
#endif
