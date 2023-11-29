## Description

This scenario simulates a constellation of two GEO satellites: Eutelsat Konnect and Eutelsat Quantum.
The satellites do not use ISL links. It is used to test beam and satellite handovers.

## Folder organization

The folder is composed of the following subfolders:

- `positions`: TLEs, ISL links, position of GWs and UTs, start date
- `antennapatterns`: this folder holds the antenna gain pattern data for each single spot-beam
- `beams`: beam description
- `standard`: standard to use. Can be either DVB or LORA
- `waveforms`: return link waveforms