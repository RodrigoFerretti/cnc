#include "arc.cpp"
#include "unity.h"

void clockWiseCircle()
{
    double arcPrecision = 0.02;
    double initialPosition[2] = {0, 0};
    double initialDistanceToCenter[2] = {50000, 0};
    double finalPosition[2] = {0, 0};
    double speedMagnitude = 15000;

    bool isClockWise = true;

    Arc arc = Arc(initialPosition, initialDistanceToCenter, finalPosition, speedMagnitude, isClockWise, arcPrecision);

    int segmentCount = arc.getSegmentCount();

    Arc::Segment segment = arc.getSegment(segmentCount / 2);

    TEST_ASSERT_EQUAL(segmentCount, 3512);
    TEST_ASSERT_EQUAL(segment.position[0], 100000);
    TEST_ASSERT_EQUAL(segment.position[1], 0);
}

void counterClockWiseHalfCircle()
{
    double arcPrecision = 0.02;
    double initialPosition[2] = {20000, 20000};
    double initialDistanceToCenter[2] = {50000, 0};
    double finalPosition[2] = {120000, 20000};
    double speedMagnitude = 15000;

    bool isClockWise = false;

    Arc arc = Arc(initialPosition, initialDistanceToCenter, finalPosition, speedMagnitude, isClockWise, arcPrecision);

    int segmentCount = arc.getSegmentCount();

    Arc::Segment segment = arc.getSegment(segmentCount / 2);

    TEST_ASSERT_EQUAL(segmentCount, 1756);
    TEST_ASSERT_EQUAL(segment.position[0], 70000);
    TEST_ASSERT_EQUAL(segment.position[1], -30000);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(clockWiseCircle);
    RUN_TEST(counterClockWiseHalfCircle);
    UNITY_END();

    return 0;
}