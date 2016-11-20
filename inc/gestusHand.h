#ifndef HAND
#define HAND


struct phalange
{
    int id;
    double angle;
};

struct finger
{
    int id;
    phanlage* phalanges;
};

class Hand
{
    public:
        Hand();
        ~Hand();
        Hand(const Hand&);
        Hand&  operator=(const Hand&);

        getId();
        setId();

        getPosition();
        setPosition();

        getFingers();
        setFingers();
    private:
        string id;
        double position[3];
        finger* fingers;
};

#endif
