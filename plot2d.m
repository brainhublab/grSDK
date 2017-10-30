close all;
DATA_FILE = "/home/peio/src/gestusSDK/build/firs_acc.txt";
data = load(DATA_FILE);

n_rows = rows(data);
time_axis = rot90(1:n_rows/5, -1);

%plot(time_axis, data(:,1), 'Color', 'red', ...
%     time_axis, data(:,2), 'Color', 'green', ...
%     time_axis, data(:,3), 'Color', 'blue');

%plot(time_axis, (data(:,1).^2 + data(:,2).^2 + data(:,3).^2), 'Color', 'red', ...
%     time_axis, (data(:,4).^2 + data(:,5).^2 + data(:,6).^2)/1000000, 'Color', 'green');

% plot(time_axis, (data(:,1).^2 + data(:,2).^2 + data(:,3).^2), 'Color', 'red');

arr = []
for i=(1:5:(rows(data)-5))
    sum = 0;
    sum2 = 0;
    for j=0:4
        sum = sum + (data(i+j,1).^2 + data(i+j,2).^2 + data(i+j,3).^2);
    endfor
    sum = sum/5;
    for j=0:4
        sum2 = sum2 + ((data(i+j,1).^2 + data(i+j,2).^2 + data(i+j,3).^2) - sum)^2;
    endfor
    arr = [arr;sum2/5];
endfor
plot(time_axis, arr, 'Color', 'red');