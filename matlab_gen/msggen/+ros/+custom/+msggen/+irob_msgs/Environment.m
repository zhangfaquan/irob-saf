classdef Environment < ros.Message
    %Environment MATLAB implementation of irob_msgs/Environment
    %   This class was automatically generated by
    %   ros.msg.internal.gen.MessageClassGenerator.
    
    %   Copyright 2014-2019 The MathWorks, Inc.
    
    %#ok<*INUSD>
    
    properties (Constant)
        MessageType = 'irob_msgs/Environment' % The ROS message type
    end
    
    properties (Constant, Hidden)
        MD5Checksum = '0d4d36bba3c5e21fb3144f87836deecd' % The MD5 Checksum of the message definition
    end
    
    properties (Access = protected)
        JavaMessage % The Java message object
    end
    
    properties (Constant)
        VALID = int8(1)
        INVALID = int8(2)
    end
    
    properties (Constant, Access = protected)
        GeometryMsgsTransformClass = ros.msg.internal.MessageFactory.getClassForType('geometry_msgs/Transform') % Dispatch to MATLAB class for message type geometry_msgs/Transform
        IrobMsgsGraspObjectClass = ros.msg.internal.MessageFactory.getClassForType('irob_msgs/GraspObject') % Dispatch to MATLAB class for message type irob_msgs/GraspObject
        StdMsgsHeaderClass = ros.msg.internal.MessageFactory.getClassForType('std_msgs/Header') % Dispatch to MATLAB class for message type std_msgs/Header
    end
    
    properties (Dependent)
        Header
        Valid
        TfPhantom
        Objects
    end
    
    properties (Access = protected)
        Cache = struct('Header', [], 'TfPhantom', [], 'Objects', []) % The cache for fast data access
    end
    
    properties (Constant, Hidden)
        PropertyList = {'Header', 'Objects', 'TfPhantom', 'Valid'} % List of non-constant message properties
        ROSPropertyList = {'header', 'objects', 'tf_phantom', 'valid'} % List of non-constant ROS message properties
    end
    
    methods
        function obj = Environment(msg)
            %Environment Construct the message object Environment
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
            validateattributes(header, {obj.StdMsgsHeaderClass}, {'nonempty', 'scalar'}, 'Environment', 'Header');
            
            obj.JavaMessage.setHeader(header.getJavaObject);
            
            % Update cache if necessary
            if ~isempty(obj.Cache.Header)
                obj.Cache.Header.setJavaObject(header.getJavaObject);
            end
        end
        
        function valid = get.Valid(obj)
            %get.Valid Get the value for property Valid
            valid = int8(obj.JavaMessage.getValid);
        end
        
        function set.Valid(obj, valid)
            %set.Valid Set the value for property Valid
            validateattributes(valid, {'numeric'}, {'nonempty', 'scalar'}, 'Environment', 'Valid');
            
            obj.JavaMessage.setValid(valid);
        end
        
        function tfphantom = get.TfPhantom(obj)
            %get.TfPhantom Get the value for property TfPhantom
            if isempty(obj.Cache.TfPhantom)
                obj.Cache.TfPhantom = feval(obj.GeometryMsgsTransformClass, obj.JavaMessage.getTfPhantom);
            end
            tfphantom = obj.Cache.TfPhantom;
        end
        
        function set.TfPhantom(obj, tfphantom)
            %set.TfPhantom Set the value for property TfPhantom
            validateattributes(tfphantom, {obj.GeometryMsgsTransformClass}, {'nonempty', 'scalar'}, 'Environment', 'TfPhantom');
            
            obj.JavaMessage.setTfPhantom(tfphantom.getJavaObject);
            
            % Update cache if necessary
            if ~isempty(obj.Cache.TfPhantom)
                obj.Cache.TfPhantom.setJavaObject(tfphantom.getJavaObject);
            end
        end
        
        function objects = get.Objects(obj)
            %get.Objects Get the value for property Objects
            if isempty(obj.Cache.Objects)
                javaArray = obj.JavaMessage.getObjects;
                array = obj.readJavaArray(javaArray, obj.IrobMsgsGraspObjectClass);
                obj.Cache.Objects = feval(obj.IrobMsgsGraspObjectClass, array);
            end
            objects = obj.Cache.Objects;
        end
        
        function set.Objects(obj, objects)
            %set.Objects Set the value for property Objects
            if ~isvector(objects) && isempty(objects)
                % Allow empty [] input
                objects = feval([obj.IrobMsgsGraspObjectClass '.empty'], 0, 1);
            end
            
            validateattributes(objects, {obj.IrobMsgsGraspObjectClass}, {'vector'}, 'Environment', 'Objects');
            
            javaArray = obj.JavaMessage.getObjects;
            array = obj.writeJavaArray(objects, javaArray, obj.IrobMsgsGraspObjectClass);
            obj.JavaMessage.setObjects(array);
            
            % Update cache if necessary
            if ~isempty(obj.Cache.Objects)
                obj.Cache.Objects = [];
                obj.Cache.Objects = obj.Objects;
            end
        end
    end
    
    methods (Access = protected)
        function resetCache(obj)
            %resetCache Resets any cached properties
            obj.Cache.Header = [];
            obj.Cache.TfPhantom = [];
            obj.Cache.Objects = [];
        end
        
        function cpObj = copyElement(obj)
            %copyElement Implements deep copy behavior for message
            
            % Call default copy method for shallow copy
            cpObj = copyElement@ros.Message(obj);
            
            % Clear any existing cached properties
            cpObj.resetCache;
            
            % Create a new Java message object
            cpObj.JavaMessage = obj.createNewJavaMessage;
            
            % Iterate over all primitive properties
            cpObj.Valid = obj.Valid;
            
            % Recursively copy compound properties
            cpObj.Header = copy(obj.Header);
            cpObj.TfPhantom = copy(obj.TfPhantom);
            cpObj.Objects = copy(obj.Objects);
        end
        
        function reload(obj, strObj)
            %reload Called by loadobj to assign properties
            obj.Valid = strObj.Valid;
            obj.Header = feval([obj.StdMsgsHeaderClass '.loadobj'], strObj.Header);
            obj.TfPhantom = feval([obj.GeometryMsgsTransformClass '.loadobj'], strObj.TfPhantom);
            ObjectsCell = arrayfun(@(x) feval([obj.IrobMsgsGraspObjectClass '.loadobj'], x), strObj.Objects, 'UniformOutput', false);
            obj.Objects = vertcat(ObjectsCell{:});
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
            
            strObj.Valid = obj.Valid;
            strObj.Header = saveobj(obj.Header);
            strObj.TfPhantom = saveobj(obj.TfPhantom);
            strObj.Objects = arrayfun(@(x) saveobj(x), obj.Objects);
        end
    end
    
    methods (Static, Access = {?matlab.unittest.TestCase, ?ros.Message})
        function obj = loadobj(strObj)
            %loadobj Implements loading of message from MAT file
            
            % Return an empty object array if the structure element is not defined
            if isempty(strObj)
                obj = ros.custom.msggen.irob_msgs.Environment.empty(0,1);
                return
            end
            
            % Create an empty message object
            obj = ros.custom.msggen.irob_msgs.Environment;
            obj.reload(strObj);
        end
    end
end
