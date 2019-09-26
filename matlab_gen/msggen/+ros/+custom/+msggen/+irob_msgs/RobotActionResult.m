classdef RobotActionResult < ros.Message
    %RobotActionResult MATLAB implementation of irob_msgs/RobotActionResult
    %   This class was automatically generated by
    %   ros.msg.internal.gen.MessageClassGenerator.
    
    %   Copyright 2014-2019 The MathWorks, Inc.
    
    %#ok<*INUSD>
    
    properties (Constant)
        MessageType = 'irob_msgs/RobotActionResult' % The ROS message type
    end
    
    properties (Constant, Hidden)
        MD5Checksum = '52cc7c17839ec36f9be1834095751fc0' % The MD5 Checksum of the message definition
    end
    
    properties (Access = protected)
        JavaMessage % The Java message object
    end
    
    properties (Constant, Access = protected)
        ActionlibMsgsGoalStatusClass = ros.msg.internal.MessageFactory.getClassForType('actionlib_msgs/GoalStatus') % Dispatch to MATLAB class for message type actionlib_msgs/GoalStatus
        IrobMsgsRobotResultClass = ros.msg.internal.MessageFactory.getClassForType('irob_msgs/RobotResult') % Dispatch to MATLAB class for message type irob_msgs/RobotResult
        StdMsgsHeaderClass = ros.msg.internal.MessageFactory.getClassForType('std_msgs/Header') % Dispatch to MATLAB class for message type std_msgs/Header
    end
    
    properties (Dependent)
        Header
        Status
        Result
    end
    
    properties (Access = protected)
        Cache = struct('Header', [], 'Status', [], 'Result', []) % The cache for fast data access
    end
    
    properties (Constant, Hidden)
        PropertyList = {'Header', 'Result', 'Status'} % List of non-constant message properties
        ROSPropertyList = {'header', 'result', 'status'} % List of non-constant ROS message properties
    end
    
    methods
        function obj = RobotActionResult(msg)
            %RobotActionResult Construct the message object RobotActionResult
            import com.mathworks.toolbox.ros.message.MessageInfo;
            
            % Support default constructor
            if nargin == 0
                obj.JavaMessage = obj.createNewJavaMessage;
                return;
            end
            
            % Construct appropriate empty array
            if isempty(msg)
                obj = obj.empty(0,1);
                return;
            end
            
            % Make scalar construction fast
            if isscalar(msg)
                % Check for correct input class
                if ~MessageInfo.compareTypes(msg(1), obj.MessageType)
                    error(message('ros:mlros:message:NoTypeMatch', obj.MessageType, ...
                        char(MessageInfo.getType(msg(1))) ));
                end
                obj.JavaMessage = msg(1);
                return;
            end
            
            % Check that this is a vector of scalar messages. Since this
            % is an object array, use arrayfun to verify.
            if ~all(arrayfun(@isscalar, msg))
                error(message('ros:mlros:message:MessageArraySizeError'));
            end
            
            % Check that all messages in the array have the correct type
            if ~all(arrayfun(@(x) MessageInfo.compareTypes(x, obj.MessageType), msg))
                error(message('ros:mlros:message:NoTypeMatchArray', obj.MessageType));
            end
            
            % Construct array of objects if necessary
            objType = class(obj);
            for i = 1:length(msg)
                obj(i,1) = feval(objType, msg(i)); %#ok<AGROW>
            end
        end
        
        function header = get.Header(obj)
            %get.Header Get the value for property Header
            if isempty(obj.Cache.Header)
                obj.Cache.Header = feval(obj.StdMsgsHeaderClass, obj.JavaMessage.getHeader);
            end
            header = obj.Cache.Header;
        end
        
        function set.Header(obj, header)
            %set.Header Set the value for property Header
            validateattributes(header, {obj.StdMsgsHeaderClass}, {'nonempty', 'scalar'}, 'RobotActionResult', 'Header');
            
            obj.JavaMessage.setHeader(header.getJavaObject);
            
            % Update cache if necessary
            if ~isempty(obj.Cache.Header)
                obj.Cache.Header.setJavaObject(header.getJavaObject);
            end
        end
        
        function status = get.Status(obj)
            %get.Status Get the value for property Status
            if isempty(obj.Cache.Status)
                obj.Cache.Status = feval(obj.ActionlibMsgsGoalStatusClass, obj.JavaMessage.getStatus);
            end
            status = obj.Cache.Status;
        end
        
        function set.Status(obj, status)
            %set.Status Set the value for property Status
            validateattributes(status, {obj.ActionlibMsgsGoalStatusClass}, {'nonempty', 'scalar'}, 'RobotActionResult', 'Status');
            
            obj.JavaMessage.setStatus(status.getJavaObject);
            
            % Update cache if necessary
            if ~isempty(obj.Cache.Status)
                obj.Cache.Status.setJavaObject(status.getJavaObject);
            end
        end
        
        function result = get.Result(obj)
            %get.Result Get the value for property Result
            if isempty(obj.Cache.Result)
                obj.Cache.Result = feval(obj.IrobMsgsRobotResultClass, obj.JavaMessage.getResult);
            end
            result = obj.Cache.Result;
        end
        
        function set.Result(obj, result)
            %set.Result Set the value for property Result
            validateattributes(result, {obj.IrobMsgsRobotResultClass}, {'nonempty', 'scalar'}, 'RobotActionResult', 'Result');
            
            obj.JavaMessage.setResult(result.getJavaObject);
            
            % Update cache if necessary
            if ~isempty(obj.Cache.Result)
                obj.Cache.Result.setJavaObject(result.getJavaObject);
            end
        end
    end
    
    methods (Access = protected)
        function resetCache(obj)
            %resetCache Resets any cached properties
            obj.Cache.Header = [];
            obj.Cache.Status = [];
            obj.Cache.Result = [];
        end
        
        function cpObj = copyElement(obj)
            %copyElement Implements deep copy behavior for message
            
            % Call default copy method for shallow copy
            cpObj = copyElement@ros.Message(obj);
            
            % Clear any existing cached properties
            cpObj.resetCache;
            
            % Create a new Java message object
            cpObj.JavaMessage = obj.createNewJavaMessage;
            
            % Recursively copy compound properties
            cpObj.Header = copy(obj.Header);
            cpObj.Status = copy(obj.Status);
            cpObj.Result = copy(obj.Result);
        end
        
        function reload(obj, strObj)
            %reload Called by loadobj to assign properties
            obj.Header = feval([obj.StdMsgsHeaderClass '.loadobj'], strObj.Header);
            obj.Status = feval([obj.ActionlibMsgsGoalStatusClass '.loadobj'], strObj.Status);
            obj.Result = feval([obj.IrobMsgsRobotResultClass '.loadobj'], strObj.Result);
        end
    end
    
    methods (Access = ?ros.Message)
        function strObj = saveobj(obj)
            %saveobj Implements saving of message to MAT file
            
            % Return an empty element if object array is empty
            if isempty(obj)
                strObj = struct.empty;
                return
            end
            
            strObj.Header = saveobj(obj.Header);
            strObj.Status = saveobj(obj.Status);
            strObj.Result = saveobj(obj.Result);
        end
    end
    
    methods (Static, Access = {?matlab.unittest.TestCase, ?ros.Message})
        function obj = loadobj(strObj)
            %loadobj Implements loading of message from MAT file
            
            % Return an empty object array if the structure element is not defined
            if isempty(strObj)
                obj = ros.custom.msggen.irob_msgs.RobotActionResult.empty(0,1);
                return
            end
            
            % Create an empty message object
            obj = ros.custom.msggen.irob_msgs.RobotActionResult;
            obj.reload(strObj);
        end
    end
end
