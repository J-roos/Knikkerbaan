#include <Servo.h>

/* Bedient een servo en duwt daarmee een blokje. */
class duwer {
	private:
		Servo servo;
		int pin = 0;
	
	public:
		float rotatie; /* 0 tot 1.0f */

		duwer() { }
		duwer(int __pin) {
			servo.attach(__pin);
			pin = __pin;
			rotatie = 0.0f;
		}

		void update() {
			/* Hou binnen 0 en 1 */
			float margin = 0.03f;		
			if (rotatie > 1.0f - margin) 
				rotatie = 1.0f - margin;
			else if (rotatie < margin) 
				rotatie = margin;

			float hoek = rotatie * 180.0f;
			servo.write((int)hoek);
		}
};

/* Grootte van het veld hier als constante. */
#define VELD_BREEDTE 4
#define VELD_HOOGTE 4

/* Bedient alle servo's. */
class duwer_veld {
	public:
		duwer duwers[VELD_BREEDTE][VELD_HOOGTE];

		duwer_veld() {
			/* 2D veld van duwers. Pins hier in coderen.
			 * Het maakt niet uit hoe de pins verbonden zijn, de servo's
			 * zullen aan deze pins bedient worden. */
			const int pins[VELD_BREEDTE][VELD_HOOGTE] = {
				{ 0,  1,  2,  3  },
				{ 4,  5,  6,  7  },
				{ 8,  9,  10, 11 },
				{ 12, 13, 14, 15 },
			};

			for (int y = 0; y < VELD_HOOGTE; ++y){
				for (int x = 0; x < VELD_BREEDTE; ++x) {
					duwers[x][y] = duwer(pins[x][y]);
				}
			}
		}

		void update() {
			for (int y = 0; y < VELD_HOOGTE; ++y){
				for (int x = 0; x < VELD_BREEDTE; ++x) {
					duwers[x][y].update();
				}
			}
		}	
};

duwer_veld *veld = nullptr;

void setup() {
	veld = new duwer_veld();
}

float time = 0.0f;
int delayms = 20;

void loop() {
	/* Hier pin bedienen. Dit kan gedaan worden met de web app of ingebouwde
	 * functies. De arduino kan deze info opsturen naar de server die dat daan weergeeft.
	 * Het idee is dus dat de server dat kan bedienen.
	 * De code voor de arduino is dus volledig klaar, alleen is de integratie van
	 * de server nog onduidelijk. Maar dat is er niet van gekomen... */
	for (int y = 0; y < VELD_HOOGTE; ++y) {
		for (int x = 0; x < VELD_BREEDTE; ++x){
			veld->duwers[x][y].rotatie = 0.5f + 0.5f * sin((float)(x + y)/3.0f + time);
		}
	}
	veld->update();

	time += delayms / 1000.0f;
	delay(delayms);
}

/* Het hele idee was dus dat de servos op een grid zouden
 * worden aangelegd en dat we die grid als functie van X and Y en tijd kunnen veranderen.
 * Hiermee kan je golfeffecten maken enz...
 * Integratie met server is ook alleen een kwestie van sturen en ontvangen van informatie.
 * 
 * Het enige wat dus ontbrak aan ons project is de duwblokjes. */

/* Deze code is getest in een demo filmpje die ik had gemaakt.
 * Dus het werkt wel, en zou op de knikkerbaan ook functioneren mits
 * alles goed is aangesloten... */
