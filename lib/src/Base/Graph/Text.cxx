//                                               -*- C++ -*-
/**
 *  @brief Text class for plot labels
 *
 *  Copyright 2005-2025 Airbus-EDF-IMACS-ONERA-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "openturns/Text.hxx"
#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Text)

static const Factory<Text> Factory_Text;

/* Accepted text position */
std::map<String, UnsignedInteger> Text::Position;


/* Default constructor */
Text::Text()
  : DrawableImplementation()
{}

/* Default constructor */
Text::Text(const Sample & data,
           const Description & textAnnotations,
           const String & textPosition,
           const String & legend)
  : DrawableImplementation(data, legend)
{
  // Check data validity
  setData(data);
  setTextAnnotations(textAnnotations);
  if(!IsValidTextPosition(textPosition))
  {
    throw InvalidArgumentException(HERE) << "The given text position = " << textPosition << " is invalid";
  }
  textPositions_ = Description(data_.getSize(), textPosition);
  textSize_ = ResourceMap::GetAsScalar("Text-DefaultTextSize");
}

/* Constructor from complex numbers */
Text::Text(const ComplexCollection & data,
           const Description & textAnnotations,
           const String & textPosition,
           const String & legend)
  : DrawableImplementation(Sample(0, 2), legend)
{
  // Convert the complex numbers into a Sample
  const UnsignedInteger size = data.getSize();
  Sample sample(size, 2);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    sample(i, 0) = data[i].real();
    sample(i, 1) = data[i].imag();
  }
  // Check data validity
  setData(sample);
  setTextAnnotations(textAnnotations);
  if(!IsValidTextPosition(textPosition))
  {
    throw InvalidArgumentException(HERE) << "The given text position = " << textPosition << " is invalid";
  }
  textPositions_ = Description(data_.getSize(), textPosition);
  textSize_ = ResourceMap::GetAsScalar("Text-DefaultTextSize");
}

/* Constructor from 2 data sets */
Text::Text(const Sample & dataX,
           const Sample & dataY,
           const Description & textAnnotations,
           const String & textPosition,
           const String & legend)
  : DrawableImplementation(Sample(0, 2), legend)
{
  const UnsignedInteger size = dataX.getSize();
  if (dataY.getSize() != size) throw InvalidArgumentException(HERE) << "Error: cannot build a Text based on two samples with different size.";
  if ((dataX.getDimension() != 1) || (dataY.getDimension() != 1)) throw InvalidDimensionException(HERE) << "Error: cannot build a Text based on two samples of dimension greater than 1.";
  Sample dataFull(dataX);
  dataFull.stack(dataY);
  // Check data validity
  setData(dataFull);
  setTextAnnotations(textAnnotations);
  if(!IsValidTextPosition(textPosition))
  {
    throw InvalidArgumentException(HERE) << "The given text position = " << textPosition << " is invalid";
  }
  textPositions_ = Description(data_.getSize(), textPosition);
  textSize_ = ResourceMap::GetAsScalar("Text-DefaultTextSize");
}

Text::Text(const Point & dataX,
           const Point & dataY,
           const Description & textAnnotations,
           const String & textPosition,
           const String & legend)
  : DrawableImplementation(Sample(0, 2), legend)
{
  const UnsignedInteger size = dataX.getDimension();
  if (dataY.getDimension() != size) throw InvalidDimensionException(HERE) << "Error: cannot build a Text based on two points with different dimension.";
  Sample dataFull(size, 2);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    dataFull(i, 0) = dataX[i];
    dataFull(i, 1) = dataY[i];
  }
  // Check data validity
  setData(dataFull);
  setTextAnnotations(textAnnotations);
  if(!IsValidTextPosition(textPosition))
  {
    throw InvalidArgumentException(HERE) << "The given text position = " << textPosition << " is invalid";
  }
  textPositions_ = Description(data_.getSize(), textPosition);
  textSize_ = ResourceMap::GetAsScalar("Text-DefaultTextSize");
}

/* String converter */
String Text::__repr__() const
{
  OSS oss;
  oss << "class=" << Text::GetClassName()
      << " name=" << getName()
      << " derived from " << DrawableImplementation::__repr__();
  return oss;
}

/* Accessor for textAnnotations */
Description Text::getTextAnnotations() const
{
  return textAnnotations_;
}

void Text::setTextAnnotations(const Description & textAnnotations)
{
  if (textAnnotations.getSize() != data_.getSize())
  {
    throw InvalidDimensionException(HERE) << "Expected array of size " << data_.getSize() << " got " << textAnnotations.getSize();
  }
  textAnnotations_ = textAnnotations;
}

/* Accessors to text position */
Description Text::getTextPositions() const
{
  return textPositions_;
}

void Text::setTextPositions(const Description & textPositions)
{
  if (textPositions.getSize() != data_.getSize())
  {
    throw InvalidDimensionException(HERE) << "Expected array of size " << data_.getSize() << " got " << textPositions.getSize();
  }
  for (UnsignedInteger i = 0; i < textPositions.getSize(); ++i)
  {
    if(!IsValidTextPosition(textPositions[i]))
    {
      throw InvalidArgumentException(HERE) << "The given text position = " << textPositions[i] << " is invalid";
    }
  }
  textPositions_ = textPositions;
}

Scalar Text::getTextSize() const
{
  return textSize_;
}

void Text::setTextSize(const Scalar size)
{
  textSize_ = size;
}

Scalar Text::getRotation() const
{
  return rotation_;
}

void Text::setRotation(const Scalar rotation)
{
  rotation_ = rotation;
}

/* Clone method */
Text * Text::clone() const
{
  return new Text(*this);
}

/* Check validity of data */
void Text::checkData(const Sample & data) const
{
  if (data.getDimension() != 2)
  {
    throw InvalidDimensionException(HERE) << "Expected sample of dimension 2: got " << data.getDimension();
  }
}

Bool Text::IsValidTextPosition(String textPosition)
{
  InitializePositionMap();
  const std::map<String, UnsignedInteger>::const_iterator it(Position.find(textPosition));
  return (it != Position.end());
}

void Text::InitializePositionMap()
{
  if (!Position.empty()) return;
  Position["bottom"] = 1;
  Position["left"] = 2;
  Position["top"] = 3;
  Position["right"] = 4;
}

/* Method save() stores the object through the StorageManager */
void Text::save(Advocate & adv) const
{
  DrawableImplementation::save(adv);
  adv.saveAttribute( "textAnnotations_", textAnnotations_ );
  adv.saveAttribute( "textPositions_", textPositions_ );
  adv.saveAttribute( "textSize_", textSize_ );
  adv.saveAttribute( "rotation_", rotation_ );
}

/* Method load() reloads the object from the StorageManager */
void Text::load(Advocate & adv)
{
  DrawableImplementation::load(adv);
  adv.loadAttribute( "textAnnotations_", textAnnotations_ );
  adv.loadAttribute( "textPositions_", textPositions_ );
  adv.loadAttribute( "textSize_", textSize_ );
  if (adv.hasAttribute("rotation_"))
    adv.loadAttribute( "rotation_", rotation_ );
}

END_NAMESPACE_OPENTURNS
