% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/stModel.define.R
\name{stModel.define}
\alias{stModel.define}
\title{Define a spacetime model object for the `f()` call.}
\usage{
stModel.define(
  smesh,
  tmesh,
  model,
  control.priors,
  debug = FALSE,
  verbose = FALSE
)
}
\arguments{
\item{smesh}{a spatial mesh}

\item{tmesh}{a temporal mesh}

\item{model}{a three characters string to specify the
smoothness alpha (each one as integer) parameters.
Currently it considers the `102`, `121`, `202` and `220` models.}

\item{control.priors}{a named list with parameter priors.
E.g. prior.rs, prior.rt and prior.sigma
as vectors with length two (U, a) to define the
corresponding PC-prior such that
P(r_s<U)=a, P(r_t<U)=a or P(sigma>U)=a.
If a=0 then U is taken to be the fixed value of the parameter.}

\item{debug}{logical indicating if to run in debug mode.}

\item{verbose}{logical indicating if to print parameter values.}
}
\value{
objects to be used in the f() formula term in INLA.
}
\description{
Define a spacetime model object for the `f()` call.
}
\details{
See the paper.
}
