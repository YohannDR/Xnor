using System;
using System.Diagnostics;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Xnor.Core;

namespace CoreCSharpTests
{
    [TestClass]
    public class ColorTest
    {
        [TestClass]
        public class ConversionRgbaHsva
        {
            [TestMethod]
            public void Red()
            {
                Color rgba = Color.Red;
                ColorHsva hsva = ColorHsva.Red;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (Color) hsva);
            }
            
            [TestMethod]
            public void Green()
            {
                Color rgba = Color.Green;
                ColorHsva hsva = ColorHsva.Green;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (Color) hsva);
            }
            
            [TestMethod]
            public void Blue()
            {
                Color rgba = Color.Blue;
                ColorHsva hsva = ColorHsva.Blue;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (Color) hsva);
            }
            
            [TestMethod]
            public void Yellow()
            {
                Color rgba = Color.Yellow;
                ColorHsva hsva = ColorHsva.Yellow;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (Color) hsva);
            }
            
            [TestMethod]
            public void Magenta()
            {
                Color rgba = Color.Magenta;
                ColorHsva hsva = ColorHsva.Magenta;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (Color) hsva);
            }
            
            [TestMethod]
            public void LightBlue()
            {
                Color rgba = Color.LightBlue;
                ColorHsva hsva = ColorHsva.LightBlue;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (Color) hsva);
            }
            
            [TestMethod]
            public void Black()
            {
                Color rgba = Color.Black;
                ColorHsva hsva = ColorHsva.Black;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (Color) hsva);
            }
            
            [TestMethod]
            public void White()
            {
                Color rgba = Color.White;
                ColorHsva hsva = ColorHsva.White;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (Color) hsva);
            }
        }
        
        [TestMethod]
        public void EqualityRgba()
        {
            Assert.AreEqual(Color.LightBlue, Color.LightBlue);
        }
        
        [TestMethod]
        public void EqualityHsva()
        {
            Assert.AreEqual(ColorHsva.LightBlue, ColorHsva.LightBlue);
        }
    }
}
