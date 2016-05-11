with import <nixpkgs> {}; {
  glucoEnv = stdenv.mkDerivation {
    name = "gluco";
    buildInputs = [
      gnumake
      python27Full
      python27Packages.virtualenv
      python27Packages.pip
      python27Packages.platformio
      avrgcclibc
      arduino-core
      avrdude
      libusb
      libusb1
      libxml2
    ];

    LD_LIBRARY_PATH = "${libxml2}/lib:${libusb}/lib:${libusb1}/lib";
  };
}
