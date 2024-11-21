//                                               -*- C++ -*-
/**
 *  @brief The test file of class TruncatedNormal for standard methods
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
#include "openturns/OT.hxx"
#include "openturns/OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

class TestObject : public TruncatedNormal
{
public:
  TestObject() : TruncatedNormal(0.5, 3.0, -2.0, 2.0) {}
  virtual ~TestObject() {}
};


int main(int, char *[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
  {
    // Test basic functionnalities
    checkClassWithClassName<TestObject>();

    // Instantiate one distribution object
    Collection<TruncatedNormal> coll(2);
    coll[0] = TruncatedNormal(1.5, 3.0, -2.0, 5.0);
    coll[1] = TruncatedNormal(50.0, 1.0, 3.0, 4.0);

    for (UnsignedInteger num = 0; num < coll.getSize(); ++num)
    {
      TruncatedNormal distribution = coll[num];
      fullprint << "Distribution " << distribution << std::endl;
      std::cout << "Distribution " << distribution << std::endl;

      // Is this distribution elliptical ?
      fullprint << "Elliptical = " << (distribution.isElliptical() ? "true" : "false") << std::endl;

      // Is this distribution continuous ?
      fullprint << "Continuous = " << (distribution.isContinuous() ? "true" : "false") << std::endl;

      // Test for realization of distribution
      Point oneRealization = distribution.getRealization();
      fullprint << "oneRealization=" << oneRealization << std::endl;

      // Test for sampling
      UnsignedInteger size = 10000;
      Sample oneSample = distribution.getSample( size );
      fullprint << "oneSample first=" << oneSample[0] << " last=" << oneSample[size - 1] << std::endl;
      fullprint << "mean=" << oneSample.computeMean() << std::endl;
      fullprint << "covariance=" << oneSample.computeCovariance() << std::endl;
      size = 100;
      for (UnsignedInteger i = 0; i < 2; ++i)
      {
        fullprint << "Kolmogorov test for the generator, sample size=" << size << " is " << (FittingTest::Kolmogorov(distribution.getSample(size), distribution).getBinaryQualityMeasure() ? "accepted" : "rejected") << std::endl;
        size *= 10;
      }

      // Define a point
      Point point( distribution.getDimension(), 3.5 );
      fullprint << "Point= " << point << std::endl;

      // Show PDF and CDF of point
      Scalar eps = 1e-5;
      Point DDF = distribution.computeDDF( point );
      fullprint << "ddf     =" << DDF << std::endl;
      Scalar LPDF = distribution.computeLogPDF( point );
      fullprint << "log pdf=" << LPDF << std::endl;
      Scalar PDF = distribution.computePDF( point );
      fullprint << "pdf     =" << PDF << std::endl;
      fullprint << "pdf (FD)=" << (distribution.computeCDF( point + Point(1, eps) ) - distribution.computeCDF( point  + Point(1, -eps) )) / (2.0 * eps) << std::endl;
      Scalar CDF = distribution.computeCDF( point );
      fullprint << "cdf=" << CDF << std::endl;
      Scalar CCDF = distribution.computeComplementaryCDF( point );
      fullprint << "ccdf=" << CCDF << std::endl;
      Scalar Survival = distribution.computeSurvivalFunction( point );
      fullprint << "survival=" << Survival << std::endl;
      Point InverseSurvival = distribution.computeInverseSurvivalFunction(0.95);
      fullprint << "Inverse survival=" << InverseSurvival << std::endl;
      fullprint << "Survival(inverse survival)=" << distribution.computeSurvivalFunction(InverseSurvival) << std::endl;
      try
      {
        Complex CF = distribution.computeCharacteristicFunction( point[0] );
        fullprint << "characteristic function=" << CF << std::endl;
        Complex LCF = distribution.computeLogCharacteristicFunction( point[0] );
        fullprint << "log characteristic function=" << LCF << std::endl;
      }
      catch (...)
      {
      }
      Point PDFgr = distribution.computePDFGradient( point );
      fullprint << "pdf gradient     =" << PDFgr << std::endl;
      Point PDFgrFD(4);
      PDFgrFD[0] = (TruncatedNormal(distribution.getMu() + eps, distribution.getSigma(), distribution.getA(), distribution.getB()).computePDF(point) -
                    TruncatedNormal(distribution.getMu() - eps, distribution.getSigma(), distribution.getA(), distribution.getB()).computePDF(point)) / (2.0 * eps);
      PDFgrFD[1] = (TruncatedNormal(distribution.getMu(), distribution.getSigma() + eps, distribution.getA(), distribution.getB()).computePDF(point) -
                    TruncatedNormal(distribution.getMu(), distribution.getSigma() - eps, distribution.getA(), distribution.getB()).computePDF(point)) / (2.0 * eps);
      PDFgrFD[2] = (TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA() + eps, distribution.getB()).computePDF(point) -
                    TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA() - eps, distribution.getB()).computePDF(point)) / (2.0 * eps);
      PDFgrFD[3] = (TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA(), distribution.getB() + eps).computePDF(point) -
                    TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA(), distribution.getB() - eps).computePDF(point)) / (2.0 * eps);
      fullprint << "pdf gradient (FD)=" << PDFgrFD << std::endl;
      // log-pdf gradient
      Point logPDFgr = distribution.computeLogPDFGradient( point );
      fullprint << "log-pdf gradient     =" << logPDFgr << std::endl;
      Point logPDFgrFD(4);
      logPDFgrFD[0] = (TruncatedNormal(distribution.getMu() + eps, distribution.getSigma(), distribution.getA(), distribution.getB()).computeLogPDF(point) -
                       TruncatedNormal(distribution.getMu() - eps, distribution.getSigma(), distribution.getA(), distribution.getB()).computeLogPDF(point)) / (2.0 * eps);
      logPDFgrFD[1] = (TruncatedNormal(distribution.getMu(), distribution.getSigma() + eps, distribution.getA(), distribution.getB()).computeLogPDF(point) -
                       TruncatedNormal(distribution.getMu(), distribution.getSigma() - eps, distribution.getA(), distribution.getB()).computeLogPDF(point)) / (2.0 * eps);
      logPDFgrFD[2] = (TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA() + eps, distribution.getB()).computeLogPDF(point) -
                       TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA() - eps, distribution.getB()).computeLogPDF(point)) / (2.0 * eps);
      logPDFgrFD[3] = (TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA(), distribution.getB() + eps).computeLogPDF(point) -
                       TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA(), distribution.getB() - eps).computeLogPDF(point)) / (2.0 * eps);
      fullprint << "log-pdf gradient (FD)=" << logPDFgrFD << std::endl;
      Point CDFgr = distribution.computeCDFGradient( point );
      fullprint << "cdf gradient     =" << CDFgr << std::endl;
      Point CDFgrFD(4);
      CDFgrFD[0] = (TruncatedNormal(distribution.getMu() + eps, distribution.getSigma(), distribution.getA(), distribution.getB()).computeCDF(point) -
                    TruncatedNormal(distribution.getMu() - eps, distribution.getSigma(), distribution.getA(), distribution.getB()).computeCDF(point)) / (2.0 * eps);
      CDFgrFD[1] = (TruncatedNormal(distribution.getMu(), distribution.getSigma() + eps, distribution.getA(), distribution.getB()).computeCDF(point) -
                    TruncatedNormal(distribution.getMu(), distribution.getSigma() - eps, distribution.getA(), distribution.getB()).computeCDF(point)) / (2.0 * eps);
      CDFgrFD[2] = (TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA() + eps, distribution.getB()).computeCDF(point) -
                    TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA() - eps, distribution.getB()).computeCDF(point)) / (2.0 * eps);
      CDFgrFD[3] = (TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA(), distribution.getB() + eps).computeCDF(point) -
                    TruncatedNormal(distribution.getMu(), distribution.getSigma(), distribution.getA(), distribution.getB() - eps).computeCDF(point)) / (2.0 * eps);
      fullprint << "cdf gradient (FD)=" << CDFgrFD << std::endl;
      Point quantile = distribution.computeQuantile( 0.95 );
      fullprint << "quantile=" << quantile << std::endl;
      fullprint << "cdf(quantile)=" << distribution.computeCDF(quantile) << std::endl;
      // Confidence regions
      Scalar threshold;
      fullprint << "Minimum volume interval=" << distribution.computeMinimumVolumeIntervalWithMarginalProbability(0.95, threshold) << std::endl;
      fullprint << "threshold=" << threshold << std::endl;
      Scalar beta;
      LevelSet levelSet(distribution.computeMinimumVolumeLevelSetWithThreshold(0.95, beta));
      fullprint << "Minimum volume level set=" << levelSet << std::endl;
      fullprint << "beta=" << beta << std::endl;
      fullprint << "Bilateral confidence interval=" << distribution.computeBilateralConfidenceIntervalWithMarginalProbability(0.95, beta) << std::endl;
      fullprint << "beta=" << beta << std::endl;
      fullprint << "Unilateral confidence interval (lower tail)=" << distribution.computeUnilateralConfidenceIntervalWithMarginalProbability(0.95, false, beta) << std::endl;
      fullprint << "beta=" << beta << std::endl;
      fullprint << "Unilateral confidence interval (upper tail)=" << distribution.computeUnilateralConfidenceIntervalWithMarginalProbability(0.95, true, beta) << std::endl;
      fullprint << "beta=" << beta << std::endl;
      fullprint << "entropy=" << distribution.computeEntropy() << std::endl;
      fullprint << "entropy (MC)=" << -distribution.computeLogPDF(distribution.getSample(1000000)).computeMean()[0] << std::endl;
      Point mean = distribution.getMean();
      fullprint << "mean=" << mean << std::endl;
      Point standardDeviation = distribution.getStandardDeviation();
      fullprint << "standard deviation=" << standardDeviation << std::endl;
      Point skewness = distribution.getSkewness();
      fullprint << "skewness=" << skewness << std::endl;
      Point kurtosis = distribution.getKurtosis();
      fullprint << "kurtosis=" << kurtosis << std::endl;
      CovarianceMatrix covariance = distribution.getCovariance();
      fullprint << "covariance=" << covariance << std::endl;
      CovarianceMatrix correlation = distribution.getCorrelation();
      fullprint << "correlation=" << correlation << std::endl;
      CovarianceMatrix spearman = distribution.getSpearmanCorrelation();
      fullprint << "spearman=" << spearman << std::endl;
      CovarianceMatrix kendall = distribution.getKendallTau();
      fullprint << "kendall=" << kendall << std::endl;
      TruncatedNormal::PointWithDescriptionCollection parameters = distribution.getParametersCollection();
      fullprint << "parameters=" << parameters << std::endl;
      fullprint << "Standard representative=" << distribution.getStandardRepresentative().__str__() << std::endl;
    }
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
