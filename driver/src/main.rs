/// Gluco
///
/// This is the main software component for
/// [Gluco](https://github.com/nebulabio/gluco). Currently it works as
/// a driver to be run on a full-fledged computer and it talks to an
/// embedded C++/Arduino program via serial port, but in the future
/// the C++ code will be incorportated into this module and thus take
/// full advantage of the clever Rust compiler.

extern crate chrono;
extern crate serial;
extern crate time;

use std::path::Path;

use time::Duration;
use chrono::datetime::DateTime;
use chrono::offset::utc::UTC;

use serial::prelude::*;

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


const SETTINGS: serial::PortSettings = serial::PortSettings {
    baud_rate:    serial::Baud9600,
    char_size:    serial::Bits8,
    parity:       serial::ParityNone,
    stop_bits:    serial::Stop1,
    flow_control: serial::FlowNone
};


struct App<'a> {
    measurements: Vec<Measurement>,
    path:         &'a str,
}

fn main() {
    let app: App = App { 
        measurements: Vec::new(),
        path:         "/dev/ttyACM0",
    };

    let mut gluco = serial::open(app.path).unwrap();

    interact(&mut gluco).unwrap();
}

fn interact<T: SerialPort>(device: &mut T) -> serial::Result<()> {
    try!(device.configure(&SETTINGS));
    try!(device.set_timeout(Duration::seconds(1)));

    let mut buf: Vec<u8> = (0..255).collect();

    println!("reading bytes");
    try!(device.read(&mut buf[..]));

    Ok(())
}
