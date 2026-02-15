#ifndef PARSABLE_H_
#define PARSABLE_H_

#include <string>

#include "logger.h"
#include "importer.h"

class Parsable {
	public:
		Parsable() { 
			m_Ready = false;
			m_Path = "";
		}

		bool is_ready() { 
			if (!m_Ready) LOG_ERROR("Attempting action on parsable \"" << m_Path << "\" when not ready");
			return m_Ready;
		}

	protected:
		void readFile(const std::string &path, Importer &imp) {
			m_Path = path;
			m_Ready = parse(m_Path, imp);
		}

		virtual bool parse(const std::string &path, Importer &imp) = 0;

	private:
		bool m_Ready;
		std::string m_Path;
};

#endif

