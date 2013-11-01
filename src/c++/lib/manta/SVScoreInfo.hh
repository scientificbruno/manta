// -*- mode: c++; indent-tabs-mode: nil; -*-
//
// Manta
// Copyright (c) 2013 Illumina, Inc.
//
// This software is provided under the terms and conditions of the
// Illumina Open Source Software License 1.
//
// You should have received a copy of the Illumina Open Source
// Software License 1 along with this program. If not, see
// <https://github.com/sequencing/licenses/>
//

///
/// \author Chris Saunders and Xiaoyu Chen
///

#pragma once

#include "manta/SVCandidateAssemblyData.hh"
#include "manta/SVCandidate.hh"

#include <iosfwd>
#include <string>


struct SVAlignmentInfo
{
    SVAlignmentInfo(
        const SVCandidate& sv,
        const SVCandidateAssemblyData& assemblyData);

    const std::string&
    bp1ContigSeq() const
    {
        return (_bp1ContigReversed ? revContigSeq : contigSeq);
    }

    const std::string&
    bp2ContigSeq() const
    {
        return (_bp2ContigReversed ? revContigSeq : contigSeq);
    }

    const std::string&
    bp1ReferenceSeq() const
    {
        return bp1RefSeq;
    }

    const std::string&
    bp2ReferenceSeq() const
    {
        return (_isSpanning ? bp2RefSeq : bp1RefSeq);
    }

    /// do we have a enough room on either side of the breakend for
    /// both the ref and contig to make a fair split read evaluation?
    bool
    isMinBpEdge(const unsigned minEdge) const;

    friend
    std::ostream&
    operator<<(std::ostream& os, const SVAlignmentInfo& ai);

private:
    std::string contigSeq;
    std::string revContigSeq;
    std::string bp1RefSeq;
    std::string bp2RefSeq;
    const bool _isSpanning;
    const bool _bp1ContigReversed;
    const bool _bp2ContigReversed;

public:
    unsigned bp1ContigOffset;
    unsigned bp2ContigOffset;
    unsigned bp1RefOffset;
    unsigned bp2RefOffset;
};

std::ostream&
operator<<(std::ostream& os, const SVAlignmentInfo& ai);



/// sample-specific and allele-specific evidence info
struct SVSampleAlleleInfo
{
    SVSampleAlleleInfo() :
        bp1SpanReadCount(0),
        bp2SpanReadCount(0),
        spanPairCount(0),
        confidentSpanningPairCount(0),
        splitReadCount(0),
        splitReadEvidence(0),
        splitReadMapQ(0),
        confidentSplitReadCount(0)
    {}

    void
    clear()
    {
        bp1SpanReadCount = 0;
        bp2SpanReadCount = 0;
        spanPairCount = 0;
        confidentSpanningPairCount = 0;
        splitReadCount = 0;
        splitReadEvidence = 0;
        splitReadMapQ = 0;
        confidentSplitReadCount = 0;
    }

    // allele pair support
    unsigned bp1SpanReadCount;
    unsigned bp2SpanReadCount;
    unsigned spanPairCount;

    unsigned confidentSpanningPairCount;  ///< pairs where both reads are mapped and we've successfully looked up a fragment prob of 0.01 or more

    // allele split support
    unsigned splitReadCount;
    float splitReadEvidence;
    float splitReadMapQ;

    unsigned confidentSplitReadCount; ///< count by comparing alignment quality vs the other allele
};

std::ostream&
operator<<(std::ostream& os, const SVSampleAlleleInfo& si);



/// sample-specific evidence info
struct SVSampleInfo
{
    void
    clear()
    {
        alt.clear();
        ref.clear();
    }

    SVSampleAlleleInfo alt;
    SVSampleAlleleInfo ref;
};

std::ostream&
operator<<(std::ostream& os, const SVSampleInfo& si);



/// consolidate model-agnostic scoring results applied to an SV candidate
struct SVScoreInfo
{
    SVScoreInfo() :
        bp1MaxDepth(0),
        bp2MaxDepth(0)
    {}

    void
    clear()
    {
        normal.clear();
        tumor.clear();

        bp1MaxDepth=0;
        bp2MaxDepth=0;
    }

    SVSampleInfo normal;
    SVSampleInfo tumor;

    unsigned bp1MaxDepth;
    unsigned bp2MaxDepth;
};


std::ostream&
operator<<(std::ostream& os, const SVScoreInfo& ssi);
