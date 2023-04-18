#pragma once

enum drop_off{
    NO_DROPOFF=1,
    SMALL_DROPOFF=12,
    MID_DROPOFF=12,
    BIG_DROPOFF=28
};

void hailMary(short discs, drop_off dropOff=MID_DROPOFF, int time_out=-1);

void hailMarySlow(short discs, double indexRPM=-1, int time_out=-1);

extern short score_time;

void hailMaryMatchLoad(int time_out);

void hailMaryDrive(int tarShots, int tarSpeed);