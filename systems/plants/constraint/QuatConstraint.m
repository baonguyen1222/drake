classdef QuatConstraint <SingleTimeKinematicConstraint
  % Constrain the quaternion to satisfy the following conditions: 
  % (quat'*quat_des)^2 in [1-tol 1]
  properties(SetAccess = protected)
    tol
  end
  
  methods(Abstract,Access = protected)
    [orient_prod,dorient_prod] = evalOrientationProduct(obj,kinsol)
  end
  
  methods
    function obj = QuatConstraint(robot,tol,tspan)
      obj = obj@SingleTimeKinematicConstraint(robot,tspan);
      sizecheck(tol,[1,1]);
      if(tol<0||tol>1)
        error('Drake:QuatConstraint:Tol must be within the range [0 1]');
      end
      obj.tol = tol;
      obj.num_constraint = 1;
    end
    
    function [c,dc] = eval(obj,t,kinsol)
      if(obj.isTimeValid(t))
        [orient_prod, dorient_prod] = evalOrientationProduct(obj,kinsol);
        c = orient_prod^2;
        dc = 2*(orient_prod)*dorient_prod;
      else
        c = [];
        dc = [];
      end
    end
    
    function [lb,ub] = bounds(obj,t)
      if(obj.isTimeValid(t))
        lb = 1-obj.tol;
        ub = 1;
      else
        lb = [];
        ub = [];
      end
    end
    
  end
end
