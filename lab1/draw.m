clc;
clear;
close all;

out1

fig = figure();

subplot(2,1,1);

hold on;
title("Jawna metoda Eulera");
scatter(euler1t, euler1y, "filled");
scatter(euler2t, euler2y, "filled");
scatter(euler3t, euler3y, "filled")
legend("dt=0.01", "dt=0.1", "dt=1")
hold off;

subplot(2,1,2);

hold on;
title("Jawna metoda Eulera blad");
plot(euler1errt, euler1erry, 'LineWidth', 3);
plot(euler2errt, euler2erry, 'LineWidth', 3);
plot(euler3errt, euler3erry, 'LineWidth', 3)
legend("dt=0.01", "dt=0.1", "dt=1")
hold off;

set(fig, 'PaperPositionMode', 'auto')
set(fig, 'position', [0,0,1000,1000])
saveas(fig,"1", 'png')

fig = figure();

subplot(3,1,1);

hold on;
title("Metoda RK2");
scatter(rk2_1t, rk2_1y, "filled");
scatter(rk2_2t, rk2_2y, "filled");
scatter(rk2_3t, rk2_3y, "filled")
legend("dt=0.01", "dt=0.1", "dt=1")
hold off;

subplot(3,1,2);

hold on;
title("Metoda RK2 blad");
plot(rk2_1errt, rk2_1erry, 'LineWidth', 3);
plot(rk2_2errt, rk2_2erry, 'LineWidth', 3);
plot(rk2_3errt, rk2_3erry, 'LineWidth', 3)
legend("dt=0.01", "dt=0.1", "dt=1")
hold off;

subplot(3,1,3);

hold on;
title("Metoda RK2 blad - zblizony");
plot(rk2_1errt, rk2_1erry, 'LineWidth', 3);
plot(rk2_2errt, rk2_2erry, 'LineWidth', 3);
legend("dt=0.01", "dt=0.1")
hold off;

set(fig, 'position', [0,0,1000,1000])

saveas(fig,"2", 'png')

fig = figure();

subplot(3,1,1);

hold on;
title("Metoda rk4");
scatter(rk4_1t, rk4_1y, "filled");
scatter(rk4_2t, rk4_2y, "filled");
scatter(rk4_3t, rk4_3y, "filled")
legend("dt=0.01", "dt=0.1", "dt=1")
hold off;

subplot(3,1,2);

hold on;
title("Metoda rk4 blad");
plot(rk4_1errt, rk4_1erry, 'LineWidth', 3);
plot(rk4_2errt, rk4_2erry, 'LineWidth', 3);
plot(rk4_3errt, rk4_3erry, 'LineWidth', 3)
legend("dt=0.01", "dt=0.1", "dt=1")
hold off;


subplot(3,1,3);

hold on;
title("Metoda rk4 blad - zblizony");
plot(rk4_1errt, rk4_1erry, 'LineWidth', 3);
plot(rk4_2errt, rk4_2erry, 'LineWidth', 3);
legend("dt=0.01", "dt=0.1")
hold off;

set(fig, 'position', [0,0,1000,1000])

saveas(fig,"3", 'png')

fig = figure();

subplot(2,1,1);

[rk4_rlcqt,sortIdx] = sort(rk4_rlcqt,'ascend');
rk4_rlcqy = rk4_rlcqy(sortIdx);

hold on;
title("Metoda rk4 RLC - Q(t)");
plot(rk4_rlcqt, rk4_rlcqy, 'LineWidth', 3);
plot(rk4_rlcq1t, rk4_rlcq1y, 'LineWidth', 3);
plot(rk4_rlcq2t, rk4_rlcq2y, 'LineWidth', 3);
plot(rk4_rlcq3t, rk4_rlcq3y, 'LineWidth', 3);
legend("0.5 \omega 0", "0.8 \omega 0", "1.0 \omega 0", "1.2 \omega 0")
hold off;

subplot(2,1,2);

[rk4_rlcit,sortIdx] = sort(rk4_rlcit,'ascend');
rk4_rlciy = rk4_rlciy(sortIdx);

hold on;
title("Metoda rk4 RLC - I(t)");
plot(rk4_rlcit, rk4_rlciy, 'LineWidth', 3);
plot(rk4_rlci3t, rk4_rlci1y, 'LineWidth', 3);
plot(rk4_rlci2t, rk4_rlci2y, 'LineWidth', 3);
plot(rk4_rlci1t, rk4_rlci3y, 'LineWidth', 3);
legend("0.5 \omega 0", "0.8 \omega 0", "1.0 \omega 0", "1.2 \omega 0")
hold off;

set(fig, 'position', [0,0,1000,1000])

saveas(fig,"4", 'png')

clear;