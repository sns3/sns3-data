# SNS3 additional-data

This repository contains :
- `antennapatterns`: all antenna patterns are stored here. There may be several configurations. Due to the huge size of these patterns, scenarios from [scenario folder](../scenarios) use symbolic links to point to this folder, in order to avoid having to duplicate antenna patterns in several scenarios
- `ext-fadingtraces`: folder containing additional external fading traces. They can be loaded using the following attributes
    - `ns3::SatChannel::EnableExternalFadingInputTrace`
    - `ns3::SatFadingExternalInputTraceContainer::UtFwdDownIndexFileName`
    - `ns3::SatFadingExternalInputTraceContainer::UtRtnUpIndexFileName`
- `fadingtraces`: folder containing fading traces. This configuration is made by setting attribute `ns3::SatBeamHelper::FadingModel` to `SatEnums::FADING_TRACE`
- `interferencetraces`: folder containing interference traces. They can be loaded setting the following attributes to `SatPhyRxCarrierConf::IF_TRACE`
    - `ns3::SatGwHelper::DaRtnLinkInterferenceModel`
    - `ns3::SatOrbiterHelper::DaRtnLinkInterferenceModel`
    - `ns3::SatOrbiterHelper::DaFwdLinkInterferenceModel`
    - `ns3::SatUtHelper::DaFwdLinkInterferenceModel`
- `linkresults`: contains link results for all MODCOD (i.e. waveform) that can be used. Only the waveforms that can be used in a scenario are automatically loaded by the helpers, depending on the parameters (`DVB` or `LORA`, `S2` or `S2X`, etc.). Each link result contains several working points with format `SNR BLER`
- `rxpowertraces`: folder containing RX power density traces. They can be loaded setting the attribute `ns3::SatChannel::RxPowerCalculationMode` to `SatEnums::RX_PWR_INPUT_TRACE`
- `sinrmeaserror`: traces used to add a channel estimation error on top of raw measured SINR. Can be enabled with attributes
    - `ns3::SatGwHelper::EnableChannelEstimationError`
    - `ns3::SatUtHelper::EnableChannelEstimationError`
- `utpositions`: additional UT positions that can be used
    - static UT positions: overwrites UT positions defined in [scenario folder](../scenarios). They can be loaded by calling method `SimulationHelper::EnableUtListPositionsFromInputFile`
    - mobile UTs: add mobile UTs to the simulation. They are loaded by adding the mobile UTs path as a second argument of `SimulationHelper::CreateSatScenario`

Examples that use each additional data (non exhaustive list):
- `ext-fadingtraces`: `sat-trace-input-external-fading-example`
- `fadingtraces`: `sat-trace-input-fading-example`
- `interferencetraces`: `sat-trace-input-interference-example`
- `rxpowertraces`: `sat-trace-input-rx-power-example`
- `sinrmeaserror`: `sat-dama-http-sim-tn9` or `sat-ra-sim-tn9`
- `utpositions`:
    - static UT positions: `sat-list-position-ext-fading-example`
    - mobile UTs: `sat-handover-example` or `sat-mobility-beam-tracer`
