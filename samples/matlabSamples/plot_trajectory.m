close all;
DATA_FILE = "/home/h3lgi/code/grSDK/build/firs.txt";
data = load(DATA_FILE);

line(data(:,1), data(:,2), data(:,3));
title ("GR Trajectory record");
ylabel("y axis");
xlabel("x axis");
zlabel("z axis");