#define RED "100"
#define GREEN "010"
#define BLUE "001"
#define CYAN "011"
#define YELLOW "110"
#define PINK "101"
#define WHITE "111"
#define OFF "000"


class TVCLED{

    public:
        int pin1, pin2, pin3;

        TVCLED(int pin1, int pin2, int pin3);

        void begin();

        void changeColor(String color);
};