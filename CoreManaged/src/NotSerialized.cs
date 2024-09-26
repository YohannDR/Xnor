using System;

namespace Xnor.Core
{
    /// <summary>
    /// Tells the XNOR engine to avoid serializing a public field.
    /// </summary>
    [AttributeUsage(AttributeTargets.Field)]
    public class NotSerialized : Attribute
    {
    }
}
