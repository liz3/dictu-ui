#import "mac_location.h"

@implementation LocationFetcher

- (instancetype)init {
    self = [super init];
    if (self) {
        // Initialize the CLLocationManager
        self.locationManager = [[CLLocationManager alloc] init];
        self.locationManager.delegate = self;
        self.locationManager.desiredAccuracy = kCLLocationAccuracyBest;
        self.did_receive = false;
        self.did_error = false;
    }
    return self;
}

- (void)startFetchingLocation {
    // Request permission to access location
    if ([CLLocationManager locationServicesEnabled]) {
        // On macOS, we request authorization here
        [self.locationManager requestWhenInUseAuthorization];
        
        // Start updating the location
        [self.locationManager startUpdatingLocation];
    } else {
       self.did_error = true;
    }
}

#pragma mark - CLLocationManagerDelegate

// Called when location is updated
- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray<CLLocation *> *)locations {
    // Get the most recent location
    CLLocation *location = [locations lastObject];
    
    self.did_receive = true;
    self.lng = location.coordinate.longitude;
    self.lat = location.coordinate.latitude;
        NSLog(@"Latitude: %f, Longitude: %f", location.coordinate.latitude, location.coordinate.longitude);
    // Stop updating location to save power
    [self.locationManager stopUpdatingLocation];
    CFRunLoopStop(CFRunLoopGetCurrent());
}

// Called if there's an error
- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error {
      self.did_error = true;
}

@end

void getLocation(Location* loc) {
        @autoreleasepool {
            LocationFetcher *fetcher = [[LocationFetcher alloc] init];
            [fetcher startFetchingLocation];
            NSDate *startDate = [NSDate date];
            NSDate *endDate = [NSDate dateWithTimeIntervalSinceNow:5];
            NSRunLoop *runLoop = [NSRunLoop currentRunLoop];
            [runLoop runUntilDate:endDate];
            if ([fetcher did_receive] == true) {
                loc->was_set = true;
                loc->lat = [fetcher lat];
                loc->lng = [fetcher lng];
            } else {
                  loc->was_set = false;
            }
    }
}

