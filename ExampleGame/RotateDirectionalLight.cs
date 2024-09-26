using Xnor.Core;

namespace Game
{
    public class RotateDirectionalLight : ScriptComponent
    {
        [Serialized]
        private float speed = 1f;

        protected override void Update()
        {
            Vector3 rot = Transform.RotationEulerAngle;
            rot.X = Time.TotalTime;
            Transform.RotationEulerAngle = rot;
        }
    }
}
