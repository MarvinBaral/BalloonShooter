#ifndef MISSIONCONTROLCENTER_H
#define MISSIONCONTROLCENTER_H
#include "servoControl.h"
#include "main.h"

class MissionControlCenter {
	private:
		ServoControl* servoControl;
		short repeationsUntilShot;
		short shootingCounter;
		float distanceBetweenCamAndCannon;
		float y0;
		float v0;
	public:
		bool allowedToShoot;
		MissionControlCenter(ServoControl* pServoControl);
		void handleShooting();
};

#endif // MISSIONCONTROLCENTER_H
