% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/stlines.R
\name{stlines}
\alias{stlines}
\title{To visualize time series over space.}
\usage{
stlines(
  stdata,
  spatial,
  group = NULL,
  nmax.group = NULL,
  xscale = 1,
  yscale = 1,
  colour = NULL,
  ...
)
}
\arguments{
\item{stdata}{matrix with the data, each column is a location.}

\item{spatial}{an object with one of class defined in the sp package.}

\item{group}{an integer vector indicating to which spatial unit
each time series belongs to. Default is NULL and them it is assumed that
each time series belongs o each spatial unit.}

\item{nmax.group}{an integer indicating the maximum number of time series
to be plotted over each spatial unit. Default is NULL, so all will be drawn.}

\item{xscale}{numeric to define a scaling factor in the horizontal direction.}

\item{yscale}{numeric to define a scaling factor in the vertical direction.}

\item{colour}{color (may be a vector, one for each time series).
Default is NULL and it will generate colors considering the
average of each time series.
These automatic colors are defined using the `rgb()` function with `alpha=0.5`.
It considers the relative rank of each time series mean, `r`.
`r` is then used for red,  `1-r` is used for blue and
a triangular function, `1-2*|1-r/2|`, is considered for green.
That is, time series with mean among the lowest time series averages
are shown in blue and those among the highest temperatures are shown in red.
The transition from blue to red goes so that
the intermediate ones are shown in light green.}

\item{...}{further arguments to be passed for the lines function.}
}
\value{
add lines to an existing plot
}
\description{
Plot each time series over the map centered at the location.
}
\details{
Scaling the times series is needed before drawing it over the map.
 The area of the bounding box for the spatial object
 divided by the number of locations is the standard scaling factor.
 This is further multiplied by the user given \code{xcale} and \code{yscale}.
}
\section{Warning}{

 if there are too many geographical locations, it will not look good
}

