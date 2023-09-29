# SNS3 Data - Scenarios

This folder contains all the available scenarios created:

- `constellation-eutelsat-geo-2-sats-isls`: see [README](constellation-eutelsat-geo-2-sats-isls/README.md)
- `constellation-eutelsat-geo-2-sats-no-isls`: see [README](constellation-eutelsat-geo-2-sats-no-isls/README.md)
- `constellation-iridium-next-66-sats`: see [README](constellation-iridium-next-66-sats/README.md)
- `constellation-kuiper-1156-sats`: see [README](constellation-kuiper-1156-sats/README.md)
- `constellation-starlink-1584-sats`: see [README](constellation-starlink-1584-sats/README.md)
- `constellation-telesat-351-sats`: see [README](constellation-telesat-351-sats/README.md)
- `geo-33E`: see [README](geo-33E/README.md)
- `geo-33E-beam-hopping`: see [README](geo-33E-beam-hopping/README.md)
- `geo-33E-fsim`: see [README](geo-33E-fsim/README.md)
- `geo-33E-lora`: see [README](geo-33E-lora/README.md)
- `leo-iss`: see [README](leo-iss/README.md)

More details are given in the README of each scenario.

Each scenario is composed of the following folders:

- `antennapatterns`: symbolic link to the antenna patterns to use. Original folder is located in `data/additional-input/antennapatterns`
- `beams`: forward and return configuration files, used to decribe beams
- `positions`: contains positions of GWs and UTs (more UTs can be added via `SimulationHelper` and `GroupHelper`). Contains also description of satellite position: either GEO satellite position, or TLE + list of ISL + simulation start date
- `waveforms`: contains list of all waveforms used on return link
