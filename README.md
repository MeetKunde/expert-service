## Name

Expert Service

## Description

C++ server that processes the received data and sends a response in a fixed format.
It can find and deduce new geometric dependencies from given geometric scheme.

### Input format is JSON:

```
{ 
    "maxShapeId":           unsigned int,
    "pointsNumber":         unsigned int,
    "segmentsNumber":       unsigned int,
    "circlesNumber":        unsigned int,
    "linesNumber":          unsigned int,
    "points": [
        ...
        {
            "id":       unsigned int,
            "x":        float,
            "y":        float,
        }
        ...
    ],
    "segments": [
        ...
        {
            "id":       unsigned int,
            "end1Id":   unsigned int,
            "end2Id":   unsigned int,
        }
        ...
    ],
    "circles": [
        ...
        {
            "id":       unsigned int,
            "centerId": unsigned int,
            "centerX":  float,
            "centerY":  float,
            "radius":   float,
            "pointsOn": [..., unsigned int, ...],
        }
        ...
    ],
    "lines": [
        ...
        {   
            "id":       unsigned int,
            "type":     unsigned int,
            "a":        float,
            "b":        float,
            "string":   string,
            "pointsOn": [..., unsigned int, ...],
        }
        ...
    ],
    "perpendicular": [
        ...
        {
            "id1":      unsigned int,
            "id2":      unsigned int,
        }
        ...
    ],
    "parallel": [
        ...
        {
            "id1":      unsigned int,
            "id2":      unsigned int,
        }
        ...
    ],
    "equalSegments": [
        ...
        {
            "s1End1Id":      unsigned int,
            "s1End2Id":      unsigned int,
            "s2End1Id":      unsigned int,
            "s2End2Id":      unsigned int,
        }
        ...
    ],
    "equalAngles": [
        ...
        {
            "a1End1Id":      unsigned int,
            "a1VertexId":    unsigned int,
            "a1End2Id":      unsigned int,
            "a1Type":        unsigned int,
            "a2End1Id":      unsigned int,
            "a2VertexId":    unsigned int,
            "a2End2Id":      unsigned int,
            "a2Type":        unsigned int,
        }
        ...
    ],
    "lengths": [
        ...
        {
            "end1Id":       unsigned int,
            "end2Id":       unsigned int,
            "value":        string,
        }   
        ...
    ],
    "angleValues": [
        ...
        {
            "end1Id":       unsigned int,
            "vertexId":     unsigned int,
            "end2Id":       unsigned int,
            "type":         unsigned int,
            "value":        string,
        }   
        ...
    ],
    "tangentLines": [
        ...
        {
            "lineId":      unsigned int,
            "circleId":    unsigned int,
        }
        ...
    ],
    "tangentCircles": [
        ...
        {
            "id1":      unsigned int,
            "id2":      unsigned int,
        }
        ...
    ],
    "bisectorLines": [
        ...
        {
            "lineId":       unsigned int,
            "point1Id":     unsigned int,
            "vertexId":     unsigned int,
            "point2Id":     unsigned int,
            "angleType":    unsigned int,
        }   
        ...
    ],
    "midPerpendicularLines": [
        ...
        {
            "lineId":       unsigned int,
            "end1Id":       unsigned int,
            "end2Id":       unsigned int,
        }   
        ...
    ],
    "escribedCircles": [
        ...
        {
            "circleId":     unsigned int,
            "polygon":      [..., unsigned int, ...],
        }
        ...
    ],
    "inscribedCircles": [
        ...
        {
            "circleId":     unsigned int,
            "polygon":      [..., unsigned int, ...],
        }
        ...
    ],
    "circumscribedCircles": [
        ...
        {
            "circleId":     unsigned int,
            "polygon":      [..., unsigned int, ...],
        }
        ...
    ],
    "polygonTypes": [
        ...
        {
            "type":         unsigned int,
            "polygon":      [..., unsigned int, ...],
        }
        ...
    ],
    "medians": [
        ...
        {
            "s1End1Id":      unsigned int,
            "s1End2Id":      unsigned int,
            "s2End1Id":      unsigned int,
            "s2End2Id":      unsigned int,
        }
        ...
    ],
    "altitudes": [
        ...
        {
            "lineId":       unsigned int,
            "end1Id":       unsigned int,
            "end2Id":       unsigned int,
        }   
        ...
    ],
    "midSegments": [
        ...
        {
            "a1End1Id":     unsigned int,
            "a1End2Id":     unsigned int,
            "a2End1Id":     unsigned int,
            "a2End2Id":     unsigned int,
            "sEnd2":     unsigned int,
            "sEnd2":     unsigned int,
        }
        ...
    ],
}
```

### Output format is JSON:

```
{
    "points": [
        ...
        {
            "ID":               unsigned int,
            "object": {
                "id":       unsigned int,
                "x":        float,
                "y":        float,
            },
        }
        ...
    ],
    "lines": [
        ...
        {
            "ID":               unsigned int,
            "object": {
                "id":       unsigned int,
                "type":     unsigned int,
                "a":        float,
                "b":        float,
                "end1ID":   unsigned int,
                "end2ID":   unsigned int,
                "pointsOn": [..., unsigned int, ...],
		    },
        }
        ...
    ],
    "circles": [
        ...
        {
            "ID":               unsigned int,
            "object": {
                "id":       unsigned int,
                "centerId": unsigned int,
                "centerX":  float,
                "centerY":  float,
                "radius":   float,
                "pointsOn": [..., unsigned int, ...],
            },
        }
        ...
    ],
    "intersections": {
        "line_line":                3 dimensional array of unsigned ints,
        "circle_circle":            3 dimensional array of unsigned ints,
        "line_circle":              3 dimensional array of unsigned ints,
        "points_on_line_line":      3 dimensional array of unsigned ints pairs,
        "points_on_circle_circle":  3 dimensional array of unsigned ints pairs,
        "points_on_line_circle":    3 dimensional array of unsigned ints pairs,
    },
    "indexes_of_variables": [
        ...
        {
            "name":         string
            "ids":          [unsigned int, unsigned int] or [unsigned int, unsigned int, unsigned int]
        }
        ...
    ],
    "dependencies": [
        ...
        {
            "type":                 unsigned int,
            "object1":              one of available object types -> depends on type value,
            "object2":              one of available object types -> depends on type value,
            "id":                   unsigned int,
            "category":             unsigned int,
            "type":                 unsigned int,
            "reason":               unsigned int,
            "basedOn":              [..., unsigned int, ...],
            "usefullness":          unsigned int,
        }
        ...
    ],
}
```

### Available object types:

###### Formula:

```
{
    "value":        string,
    "variables",    [..., string, ...],
}
```

###### IdHolder:

```
{
    "id":           unsigned int,
}
```

###### PolygonModel:

```
{
    "verticesIds":  [..., unsigned int, ...],
}
```

###### PointsPairModel:

```
{
    "end1Id":       unsigned int,
    "end2Id":       unsigned int,
}
```

###### AngleModel:

```
{
    "point1Id:       unsigned int,
    "vertexId:       unsigned int,
    "point2Id:       unsigned int,
    "type:           unsigned int,
}
```

###### ModelsPairModel:

```
{
    "first":        object type,
    "second":       object type,
}
```

## Authors and acknowledgment

Karol Janic

## License

MIT License

## Project status

Base version ready.
