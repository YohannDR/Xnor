%module CoreNative

%typemap(cscode) XnorCore::Transform
%{
    public Vector3 Position { get => GetPosition(); set => SetPosition(value); }

    public float PositionX { get => Position.X; set => SetPositionX(value); }

    public float PositionY { get => Position.Y; set => SetPositionY(value); }

    public float PositionZ { get => Position.Z; set => SetPositionZ(value); }
    
    public Quaternion Rotation { get => GetRotation(); set => SetRotation(value); }

    public float RotationX { get => Rotation.X; set => SetRotationX(value); }

    public float RotationY { get => Rotation.Y; set => SetRotationY(value); }

    public float RotationZ { get => Rotation.Z; set => SetRotationZ(value); }

    public float RotationW { get => Rotation.W; set => SetRotationW(value); }
    
    public Vector3 RotationEulerAngle { get => GetRotationEulerAngle(); set => SetRotationEulerAngle(value); }

    public float RotationEulerAngleX { get => RotationEulerAngle.X; set => SetRotationEulerAngleX(value); }

    public float RotationEulerAngleY { get => RotationEulerAngle.Y; set => SetRotationEulerAngleY(value); }

    public float RotationEulerAngleZ { get => RotationEulerAngle.Z; set => SetRotationEulerAngleZ(value); }
    
    public Vector3 Scale { get => GetScale(); set => SetScale(value); }

    public float ScaleX { get => Scale.X; set => SetScaleX(value); }

    public float ScaleY { get => Scale.Y; set => SetScaleY(value); }

    public float ScaleZ { get => Scale.Z; set => SetScaleZ(value); }

    public bool Changed => GetChanged();

    public Vector3 Forward => GetForward();

    public Vector3 Right => GetRight();

    public Vector3 Up => GetUp();

    public override string ToString() => $"Position: {{{Position}}}, Rotation: {{{Rotation}}}, Scale: {{{Scale}}}";
%}

%rename XnorCore::Transform::worldMatrix WorldMatrix;

%csmethodmodifiers XnorCore::Transform::GetPosition "private";
%csmethodmodifiers XnorCore::Transform::SetPosition "private";
%csmethodmodifiers XnorCore::Transform::SetPositionX "private";
%csmethodmodifiers XnorCore::Transform::SetPositionY "private";
%csmethodmodifiers XnorCore::Transform::SetPositionZ "private";

%csmethodmodifiers XnorCore::Transform::GetRotation "private";
%csmethodmodifiers XnorCore::Transform::SetRotation "private";
%csmethodmodifiers XnorCore::Transform::SetRotationX "private";
%csmethodmodifiers XnorCore::Transform::SetRotationY "private";
%csmethodmodifiers XnorCore::Transform::SetRotationZ "private";
%csmethodmodifiers XnorCore::Transform::SetRotationW "private";

%csmethodmodifiers XnorCore::Transform::GetRotationEulerAngle "private";
%csmethodmodifiers XnorCore::Transform::SetRotationEulerAngle "private";
%csmethodmodifiers XnorCore::Transform::SetRotationEulerAngleX "private";
%csmethodmodifiers XnorCore::Transform::SetRotationEulerAngleY "private";
%csmethodmodifiers XnorCore::Transform::SetRotationEulerAngleZ "private";

%csmethodmodifiers XnorCore::Transform::GetScale "private";
%csmethodmodifiers XnorCore::Transform::SetScale "private";
%csmethodmodifiers XnorCore::Transform::SetScaleX "private";
%csmethodmodifiers XnorCore::Transform::SetScaleY "private";
%csmethodmodifiers XnorCore::Transform::SetScaleZ "private";

%csmethodmodifiers XnorCore::Transform::GetChanged "private";
%csmethodmodifiers XnorCore::Transform::GetForward "private";
%csmethodmodifiers XnorCore::Transform::GetRight "private";
%csmethodmodifiers XnorCore::Transform::GetUp "private";

%include "transform.hpp"
