/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

//==============================================================================
/**
    A base class implementing common functionality for Drawable classes which
    consist of some kind of filled and stroked outline.

    @see DrawablePath, DrawableRectangle
*/
class JUCE_API  DrawableShape   : public Drawable
{
protected:
    //==============================================================================
    DrawableShape();
    DrawableShape (const DrawableShape&);

public:
    /** Destructor. */
    ~DrawableShape();

    //==============================================================================
    /** A FillType wrapper that allows the gradient coordinates to be implemented using RelativePoint.
    */
    class RelativeFillType
    {
    public:
        RelativeFillType();
        RelativeFillType (const FillType& fill);
        RelativeFillType (const RelativeFillType&);
        RelativeFillType& operator= (const RelativeFillType&);

        bool operator== (const RelativeFillType&) const;
        bool operator!= (const RelativeFillType&) const;

        bool isDynamic() const;
        bool recalculateCoords (Expression::Scope* scope);

        void writeTo (ValueTree& v, ComponentBuilder::ImageProvider*, UndoManager*) const;
        bool readFrom (const ValueTree& v, ComponentBuilder::ImageProvider*);

        //==============================================================================
        FillType fill;
        RelativePoint gradientPoint1, gradientPoint2, gradientPoint3;
    };

    //==============================================================================
    /** Sets a fill type for the path.
        This colour is used to fill the path - if you don't want the path to be
        filled (e.g. if you're just drawing an outline), set this to a transparent
        colour.

        @see setPath, setStrokeFill
    */
    void setFill (const FillType& newFill);

    /** Sets a fill type for the path.
        This colour is used to fill the path - if you don't want the path to be
        filled (e.g. if you're just drawing an outline), set this to a transparent
        colour.

        @see setPath, setStrokeFill
    */
    void setFill (const RelativeFillType& newFill);

    /** Returns the current fill type.
        @see setFill
    */
    const RelativeFillType& getFill() const noexcept                { return mainFill; }

    /** Sets the fill type with which the outline will be drawn.
        @see setFill
    */
    void setStrokeFill (const FillType& newStrokeFill);

    /** Sets the fill type with which the outline will be drawn.
        @see setFill
    */
    void setStrokeFill (const RelativeFillType& newStrokeFill);

    /** Returns the current stroke fill.
        @see setStrokeFill
    */
    const RelativeFillType& getStrokeFill() const noexcept          { return strokeFill; }

    /** Changes the properties of the outline that will be drawn around the path.
        If the stroke has 0 thickness, no stroke will be drawn.
        @see setStrokeThickness, setStrokeColour
    */
    void setStrokeType (const PathStrokeType& newStrokeType);

    /** Changes the stroke thickness.
        This is a shortcut for calling setStrokeType.
    */
    void setStrokeThickness (float newThickness);

    /** Returns the current outline style. */
    const PathStrokeType& getStrokeType() const noexcept            { return strokeType; }

    /** Provides a set of dash lengths to use for stroking the path. */
    void setDashLengths (const Array<float>& newDashLengths);

    /** Returns the set of dash lengths that the path is using. */
    const Array<float>& getDashLengths() const noexcept             { return dashLengths; }

    //==============================================================================
    /** @internal */
    class FillAndStrokeState  : public  Drawable::ValueTreeWrapperBase
    {
    public:
        FillAndStrokeState (const ValueTree& state);

        ValueTree getFillState (const Identifier& fillOrStrokeType);
        RelativeFillType getFill (const Identifier& fillOrStrokeType, ComponentBuilder::ImageProvider*) const;
        void setFill (const Identifier& fillOrStrokeType, const RelativeFillType& newFill,
                      ComponentBuilder::ImageProvider*, UndoManager*);

        PathStrokeType getStrokeType() const;
        void setStrokeType (const PathStrokeType& newStrokeType, UndoManager*);

        static const Identifier type, colour, colours, fill, stroke, path, jointStyle, capStyle, strokeWidth,
                                gradientPoint1, gradientPoint2, gradientPoint3, radial, imageId, imageOpacity;
    };

    /** @internal */
    Rectangle<float> getDrawableBounds() const override;
    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    bool hitTest (int x, int y) override;
    /** @internal */
    bool replaceColour (Colour originalColour, Colour replacementColour) override;
    /** @internal */
    Path getOutlineAsPath() const override;

protected:
    //==============================================================================
    /** Called when the cached path should be updated. */
    void pathChanged();
    /** Called when the cached stroke should be updated. */
    void strokeChanged();
    /** True if there's a stroke with a non-zero thickness and non-transparent colour. */
    bool isStrokeVisible() const noexcept;
    /** Updates the details from a FillAndStrokeState object, returning true if something changed. */
    void refreshFillTypes (const FillAndStrokeState& newState, ComponentBuilder::ImageProvider*);
    /** Writes the stroke and fill details to a FillAndStrokeState object. */
    void writeTo (FillAndStrokeState& state, ComponentBuilder::ImageProvider*, UndoManager*) const;

    //==============================================================================
    PathStrokeType strokeType;
    Array<float> dashLengths;
    Path path, strokePath;

private:
    class RelativePositioner;
    RelativeFillType mainFill, strokeFill;
    ScopedPointer<RelativeCoordinatePositionerBase> mainFillPositioner, strokeFillPositioner;

    void setFillInternal (RelativeFillType& fill, const RelativeFillType& newFill,
                          ScopedPointer<RelativeCoordinatePositionerBase>& positioner);

    DrawableShape& operator= (const DrawableShape&);
};

} // namespace juce
