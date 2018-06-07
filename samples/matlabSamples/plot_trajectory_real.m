DATA_FILE = "/home/h3lgi/code/grSDK/build/firs.txt"
fid = fopen(DATA_FILE);

title ("GR Trajectory record")

hold on
grid on

%v = [0.0001 0.0001 0.0001];
%q = [ 0 0 0 0 ];
while true
  tline = fgetl(fid);
  if ~ischar(tline)
    continue;
  end
  data = str2num(tline);
  [r, c] = size(data);
  
  
  
%  n = qtransv(v, q);
  if (c == 3)
    plot3(data(1), data(2), data(3), '-o');
    line([data(1) data(1) + 0.0001], [data(2) data(2)], [data(3) data(3)], 'Color', 'red');
    line([data(1) data(1)], [data(2) data(2) + 0.0001], [data(3) data(3)], 'Color', 'green')
    line([data(1) data(1)], [data(2) data(2)], [data(3) data(3) + 0.0001], 'Color', 'blue');
  end
  
  pause(0.1);

end

fclose(fid);

