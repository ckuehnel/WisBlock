// Build payload w/ Cayenne LPP

void buildCayenneLPP(void)
{
  lpp.reset();
  lpp.addTemperature(1, ambientTemperature);
  lpp.addRelativeHumidity(2, ambientHumidity);
  lpp.addAnalogInput(3, vocIndex);
  lpp.addAnalogInput(4, noxIndex);
  lpp.addAnalogInput(5, massConcentrationPm1p0);
  lpp.addAnalogInput(6, massConcentrationPm2p5);
  lpp.addAnalogInput(7, massConcentrationPm4p0);
  lpp.addAnalogInput(8, massConcentrationPm10p0);
}
