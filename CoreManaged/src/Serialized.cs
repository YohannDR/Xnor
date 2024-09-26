using System;

namespace Xnor.Core
{
    /// <summary>
    /// Tells the XNOR engine to serialize a private field.
    /// </summary>
    [AttributeUsage(AttributeTargets.Field)]
    public class Serialized : Attribute
    {
    }
}
