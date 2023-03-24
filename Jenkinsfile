pipeline {
  agent any
  stages {
    stage('build') {
      steps {
        echo 'Building frontend'
        sh 'chmod +x scripts/build.sh'
        sh 'scripts/build.sh'
      }
    }

    stage('test') {
      steps {
        echo 'Testing frontend'
        sh 'chmod +x scripts/test.sh'
        sh 'scripts/test.sh'
      }
    }

    stage('deploy') {
      steps {
        echo 'Deploy frontend'
         sh 'chmod +x scripts/deploy.sh'
        sh 'scripts/deploy.sh'
      }
    }
  }
  post {
    success {
      sh 'ls'
      archiveArtifacts artifacts: 'frontend',
      allowEmptyArchive: true,
      fingerprint: true,
      onlyIfSuccessful: true
    }
  }
}

