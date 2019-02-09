function [out] = irCalibTransform(val,ub,lb)
ub = mean(ub);
lb = mean(lb);
out = (val-lb)/(ub-lb);
end

