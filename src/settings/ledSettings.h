#ifndef SRC_LEDSETTINGS_H_
#define SRC_LEDSETTINGS_H_

class LedSettings {
public:
	static constexpr const int LEDS_PER_STRIP = 36;
	static constexpr const int NUM_STRIPS = 5;
	static constexpr const int NUM_LEDS = NUM_STRIPS * LEDS_PER_STRIP;
};

#endif /* SRC_LEDSETTINGS_H_ */
