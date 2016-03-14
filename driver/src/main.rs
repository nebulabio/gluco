/// Gluco

// The `Measurement` struct represents every Gluco measurement.
//
// Similar to the eHealh `glucoseData` struct, except with actual semantics. I
// _think_ that `glucose` is reported as mmol/L from the glucometer sensor, so
// that's what I default to. I'm storing `Measurements` as 32-bit floats.
struct Measurement {
    // in mmol/L:
    glucose: f32,
}

// A `Measurement` can be converted to mg/dL.
impl Measurement {
    fn to_mg_dl(&self) -> f32 {
        self.glucose
        // FIXME: write method to convert mmol/L to mg/dL
    }
}


fn main() {
    let m = Measurement {
        glucose: 6.04,
    };
    println!("Your glucose measurement.");
    println!("mmol/L : {:?}", m.glucose);
    println!("mg/dL  : {:?}", m.to_mg_dl())
}


#[cfg(test)]
mod tests {
    use super::Measurement;

    #[test]
    fn test_measurements() {
        let m = Measurement {
            glucose: 6.04,
        };

        assert_eq!(m.glucose, m.to_mg_dl());
    }
}
