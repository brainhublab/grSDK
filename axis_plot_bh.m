pkg load quaternion;
close all;
DATA_FILE = "/home/peio/src/gestusSDK/build/firs.txt";
data = load(DATA_FILE);
plot3(0, 0, 0, '-o')
line(data(:,1), data(:,2), data(:,3));

x = quaternion(1, 0, 0);
y = quaternion(0, 1, 0);
z = quaternion(0, 0, 1);

for i=1:rows(data)
  d = data(i,:);
  q = quaternion(d(:,4), d(:,5), d(:,6), d(:,7));

  % rotate by q
  q_x = q * x * conj(q);
  q_y = q * y * conj(q);
  q_z = q * z * conj(q);

  % extract vector part
  rot_x = [get(q_x, 'x'), get(q_x, 'y'), get(q_x, 'z')];
  rot_y = [get(q_y, 'x'), get(q_y, 'y'), get(q_y, 'z')];
  rot_z = [get(q_z, 'x'), get(q_z, 'y'), get(q_z, 'z')];

  axis_x = [[d(:,1), d(:,2), d(:,3)]; ([d(:,1), d(:,2), d(:,3)] + rot_x)];
  axis_y = [[d(:,1), d(:,2), d(:,3)]; ([d(:,1), d(:,2), d(:,3)] + rot_y)];
  axis_z = [[d(:,1), d(:,2), d(:,3)]; ([d(:,1), d(:,2), d(:,3)] + rot_z)];

  line(axis_x(:,1), axis_x(:,2), axis_x(:,3), 'Color', 'red');
  line(axis_y(:,1), axis_y(:,2), axis_y(:,3), 'Color', 'green');
  line(axis_z(:,1), axis_z(:,2), axis_z(:,3), 'Color', 'blue');
end

ylabel("y axis");
xlabel("x axis");
zlabel("z axis");
