#import "mac_location_interface.h"
#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

// Create a class that implements CLLocationManagerDelegate
@interface LocationFetcher : NSObject <CLLocationManagerDelegate>

// Declare CLLocationManager as a property
@property (strong, nonatomic) CLLocationManager *locationManager;

@property (nonatomic, assign) BOOL did_receive;
@property (nonatomic, assign) BOOL did_error;

@property (nonatomic, assign) double lng;
@property (nonatomic, assign) double lat;

// Method to start fetching location
- (void)startFetchingLocation;

@end