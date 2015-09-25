/// Gluco
///
/// This is the main software component for
/// [Gluco](https://github.com/nebulabio/gluco). Currently it works as
/// a driver to be run on a full-fledged computer and it talks to an
/// embedded C++/Arduino program via serial port, but in the future
/// the C++ code will be incorportated into this module and thus take
/// full advantage of the clever Rust compiler.

extern crate chrono;
use chrono::datetime::DateTime;
use chrono::offset::utc::UTC;

extern crate serial;
use serial::SerialPort;

use std::path::Path;


/// The `Measurement` struct represents every Gluco measurement.
///
/// Similar to the eHealh `glucoseData` struct, except with actual
/// semantics. I _think_ that `glucose` is reported as mmol/L from the
/// glucometer sensor, so that's what I default to.
struct Measurement {
    date: DateTime<UTC>,
    // in mmol/L:
    glucose: i32,
}

/// Methods available for a Measurement.
impl Measurement {
    /// Return the glucose measurement as mg/dL
    fn to_mg_dl(&self) -> i32 {
        self.glucose
        // FIXME: write method to convert mmol/L to mg/dL
    }
    /// Return the glucose measurement as mmol/L
    fn to_mmol_l(&self) -> i32 {
        self.glucose
        // FIXME: write method to convert mg/dL to mmol/L
    }
}

/// The application state.
///
/// Represents all the measurements from the glucometer sensor as a
/// vector of `Measurement`s and includes other important info, such
/// as the serial port to connect to.
struct App {
    measurements: Vec<Measurement>,
    port: SerialPort,
}

fn main() {
    let device_path: &Path = Path::new("/dev/ttyACM0");

    let device: SerialPort;
    let mut readings: Vec<Measurement>;

    let app: App = App { measurements: readings, port: device };

    for conn in app.port.open(&device) {
        match conn {
            Err(e) => panic!("{}", e),
            Ok(conn) => println!("Got connecton: {}", conn),
        }
    }
}
