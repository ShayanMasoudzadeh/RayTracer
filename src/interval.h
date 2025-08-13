#ifndef INTERVAL_H
#define INTERVAL_H

// Represents a closed interval [min, max] on the real number line.
// Used for bounding boxes, hit ranges, and value clamping.
class interval {
    public:
    double min, max; // Bounds of the interval

    // Default constructor: creates an empty interval (min > max).
    interval() : min(+infinity), max(-infinity) {}

    // Construct interval from given min and max.
    interval(double min, double max) : min(min), max(max) {}

    // Construct the smallest interval enclosing both input intervals.
    interval(const interval& a, const interval& b) {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }
    
    // Length of the interval.
    double size() const {
        return max - min;
    }

    // Returns true if x lies within [min, max].
    bool contains(double x) const {
        return min <= x && x <= max;
    }

    // Returns true if x lies strictly inside (min, max).
    bool surrounds(double x) const {
        return min < x && x < max;
    }

    // Clamps a value to the [min, max] range.
    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    // Returns an interval expanded by delta (split equally on both sides).
    interval expand(double delta) const {
        auto padding = delta/2;
        return interval(min - padding, max + padding);
    }

    // Predefined constants for convenience.
    static const interval empty, universe;
};

// Empty interval constant: no values inside.
const interval interval::empty    = interval(+infinity, -infinity);

// Universe interval: contains all real numbers.
const interval interval::universe = interval(-infinity, +infinity);

#endif
