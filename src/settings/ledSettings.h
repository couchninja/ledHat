#ifndef SRC_LEDSETTINGS_H_
#define SRC_LEDSETTINGS_H_

class LedSettings {
public:
	static constexpr const int LEDS_PER_STRIP = 90;
	static constexpr const int NUM_STRIPS = 1;
	static constexpr const int NUM_LEDS = NUM_STRIPS * LEDS_PER_STRIP;
};

#endif /* SRC_LEDSETTINGS_H_ */
