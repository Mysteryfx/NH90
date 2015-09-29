#ifndef __BREAKABLE_H__
#define __BREAKABLE_H__

#include "Base.h"

#include <ed/vector.h>
#include <ed/map.h>

#include "FMMathematics/ProbabilityChecker.h"

namespace EagleFM
{
	class FMBASE_API Breakable
	{
	public:
		typedef ed::vector<double> BreakableParamVec;
		typedef ed::vector<ed::string> FailuresVec;

		Breakable(unsigned int SysFailureCount = 0);

		virtual ~Breakable();

		FailuresVec& get_damages() { return o_failures_; }

		void setFailureProbability(const ed::string& Name ,double Probability, double TimeStart, double TimeInterval);
		
		/*set runtime failure probability */
		void setRTfailure(unsigned int sys_Fail_ID, double Probability, double TimeStart, double TimeInterval);

		void repairFailures();
		void simulate_planed_failures(double dt);

		/* direct damage set */
		void setDamage(int failure_id);
		void setDamage(int failure_id, const BreakableParamVec& params);

		void setImmortal(bool Immortal) {immortal = Immortal;}
        bool isImmortal () { return immortal; }

		bool isDamaged() const { return !immortal && damaged_; }

		bool getFailState(unsigned int failure_id);

	protected:

		virtual void set_planned_damage(int failure_id) = 0;
		virtual void failures_init() = 0;
		virtual bool start_damage(int failure_id, const BreakableParamVec& params) = 0;

		/*link redactor damage name with a system damage */
		void addDamageToList(const ed::string& name, unsigned failure_id);
		
		void set_failure_probability(unsigned int failure_id, double Probability, double TimeStart, double TimeInterval);

		void reportDamage(int failure_id);

		/*runtime fault witch prob checkers*/
		void initRTfailure(unsigned int sys_Fail_ID, double dt);

		void resetRTprob(unsigned int sys_Fail_ID);
		void modifyRTprob(unsigned int sys_Fail_ID, double Probability, double TimeInterval);

	private:

		virtual void repair() = 0;

		struct SystemFailure
		{
			SystemFailure() : 
				occured_(false), checker_ptr_(nullptr), rt_checker_(nullptr) {}

			~SystemFailure() {
				if(rt_checker_) delete rt_checker_;
				if(checker_ptr_) delete checker_ptr_;
			}

			EagleFM::ProbabilityChecker* checker_ptr_; /*planed damage checker*/
			EagleFM::ProbabilityChecker* rt_checker_; /* runtime checker */

			bool occured_;
		};

		typedef ed::vector<SystemFailure> SystemFailures;
		SystemFailures failures_; /*all damages*/

		FailuresVec o_failures_; /*occured failures*/
		ed::map<unsigned int, ed::string> trans_map_;

		bool immortal;
		bool damaged_;
	};
}

#endif